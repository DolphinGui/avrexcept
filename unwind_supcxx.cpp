#include <cstdint>
#include <cstdio>
#include <exception>
#include <typeinfo>

struct _Unwind_Exception;
typedef enum {
  _URC_NO_REASON = 0,
  _URC_FOREIGN_EXCEPTION_CAUGHT = 1,
  _URC_FATAL_PHASE2_ERROR = 2,
  _URC_FATAL_PHASE1_ERROR = 3,
  _URC_NORMAL_STOP = 4,
  _URC_END_OF_STACK = 5,
  _URC_HANDLER_FOUND = 6,
  _URC_INSTALL_CONTEXT = 7,
  _URC_CONTINUE_UNWIND = 8
} _Unwind_Reason_Code;

typedef uint8_t _Unwind_Exception_Class[8];

typedef void (*_Unwind_Exception_Cleanup_Fn)(_Unwind_Reason_Code,
                                             struct _Unwind_Exception *);

typedef unsigned _Unwind_Word __attribute__((__mode__(__unwind_word__)));

typedef struct _Unwind_Exception {
  _Unwind_Exception_Class exception_class;
  _Unwind_Exception_Cleanup_Fn exception_cleanup;

  _Unwind_Word current_pc; // current pc being unwound
  _Unwind_Word private_2;
} __attribute__((__aligned__)) _Unwind_Exception;

struct __cxa_exception {
  std::type_info *exceptionType;
  void(_GLIBCXX_CDTOR_CALLABI *exceptionDestructor)(void *);

  std::terminate_handler unexpectedHandler;
  std::terminate_handler terminateHandler;

  __cxa_exception *nextException;
  int handlerCount;
  int handlerSwitchValue;
  const unsigned char *actionRecord;
  const unsigned char *languageSpecificData;
  void *catchTemp;
  void *adjustedPtr;

  _Unwind_Exception unwindHeader;
};

struct __cxa_dependent_exception {
  void *primaryException;
  void(_GLIBCXX_CDTOR_CALLABI *__padding)(void *);
  std::terminate_handler unexpectedHandler;
  std::terminate_handler terminateHandler;
  __cxa_exception *nextException;
  int handlerCount;
  int handlerSwitchValue;
  const unsigned char *actionRecord;
  const unsigned char *languageSpecificData;
  void *catchTemp;
  void *adjustedPtr;

  _Unwind_Exception unwindHeader;
};

static inline bool __is_dependent_exception(_Unwind_Exception_Class c) {
  return c[7] == '\x01';
}

static inline __cxa_dependent_exception *
__get_dependent_exception_from_ue(_Unwind_Exception *exc) {
  return reinterpret_cast<__cxa_dependent_exception *>(exc + 1) - 1;
}

static inline void *__get_object_from_ue(_Unwind_Exception *eo) throw() {
  return __is_dependent_exception(eo->exception_class)
             ? __get_dependent_exception_from_ue(eo)->primaryException
             : eo + 1;
}

static inline __cxa_exception *
__get_exception_header_from_ue(_Unwind_Exception *exc) {
  return reinterpret_cast<__cxa_exception *>(exc + 1) - 1;
}

static void *get_adjusted_ptr(const void *c, const std::type_info *t,
                              void *thrown_ptr) noexcept {
  auto catch_type = static_cast<const std::type_info *>(c);
  auto throw_type = static_cast<const std::type_info *>(t);
  if (throw_type->__is_pointer_p())
    thrown_ptr = *(void **)thrown_ptr;

  if (catch_type->__do_catch(throw_type, &thrown_ptr, 1)) {
    return thrown_ptr;
  }

  return nullptr;
}

extern "C" void *get_ptr(void *exc, const std::type_info *catch_type) {
  auto ue = static_cast<_Unwind_Exception *>(exc);
  auto cxa_except = __get_exception_header_from_ue(ue);
  auto thrown_obj = __get_object_from_ue(ue);
  auto thrown_type = cxa_except->exceptionType;
  cxa_except->adjustedPtr =
      get_adjusted_ptr(catch_type, thrown_type, thrown_obj);
  return cxa_except->adjustedPtr;
}

extern "C" void __fae_terminate() { std::terminate(); }