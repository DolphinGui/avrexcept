#include <cstdint>
#include <cstdio>
#include <exception>
#include <typeinfo>

struct __class_type_info;

extern "C" void *get_adjusted_ptr(const std::type_info *catch_type,
                                  const std::type_info *throw_type,
                                  void *thrown_ptr) noexcept {
  if (throw_type->__is_pointer_p())
    thrown_ptr = *(void **)thrown_ptr;

  if (catch_type->__do_catch(throw_type, &thrown_ptr, 1)) {
    return thrown_ptr;
  }

  return nullptr;
}

extern "C" void print_typename(const std::type_info *type) noexcept {
  std::printf("typename: %s\n", type->name());
}

extern "C" void print_int() {
  auto &n = typeid(int);
  std::printf("int address: %x", (uint16_t)&n);
}

extern "C" void __fae_terminate() { std::terminate(); }