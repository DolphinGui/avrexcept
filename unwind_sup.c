#include "fae.h"
#include "stdint.h"
#include <avr/pgmspace.h>
#include <stdio.h>

#ifndef NDEBUG
#define dbg(...) printf(__VA_ARGS__)
#else
#define dbg(...)
#endif

extern prog_byte __lsda_begin[];
extern prog_byte __fae_table_start[];
extern prog_byte __fae_table_stop[];

void __fae_terminate() __attribute__((noreturn));

typedef struct table_data {
  prog_byte *data;      // r18-19
  prog_byte *data_end;  // r20-21
  uint16_t landing_pad; // r22-23
  uint8_t type_index;   // r24
} table_data;

static uint8_t uleb(prog_byte *ptr, uint16_t *out) {
  *out = *ptr++;
  if (!(*out & 0b10000000)) {
    return 1;
  } else {
    *out &= 0b01111111;
    if (*ptr & 0b00000001) {
      *out |= 0b10000000;
    }
    *out |= *ptr << 7;
    return 2;
  }
}

// here because for some reason g++ emits symbols for it directly in cfi
// directives Really should rewrite gxx_personality instead of doing it here,
// but for now this works
void __gxx_personality_v0() {}
// no plans to implement forced unwinding
void _Unwind_ForcedUnwind() { __fae_terminate(); }

static uint8_t sleb(prog_byte *data, int16_t *const val) {
  int16_t i = 0;

  uint8_t b = data[i++];
  *val = b & 0b01111111;

  if (b & 0b10000000) {
    b = data[i++];
    *val <<= 7;
    *val |= b & 0b01111111;
  }

  if (b & 0x40) {
    *val |= (-1ULL) << 7;
    return i;
  }
  return i;
}
void print_typename(const void *type);
void *__fae_get_adjusted_exc(void *exc, const void *catch_type);

// this is very bad, and probably should be rewritten in assembly for speed/size
static uint8_t personality(prog_byte *ptr, uint16_t pc_offset, void *exc,
                           uint16_t *lp_out) {
  dbg("personality called\n");
  typedef void *const __flash prog_ptr;
  uint8_t lp_encoding = *ptr++;
  uint16_t lp_offset = 0;
  if (lp_encoding != DW_EH_PE_omit) {
    ptr += uleb(ptr, &lp_offset);
  }
  uint8_t type_encoding = *ptr++;
  uint16_t types_offset = 0;
  if (type_encoding != DW_EH_PE_omit) {
    ptr += uleb(ptr, &types_offset);
  }
  prog_ptr *type_table = (prog_ptr *)(ptr + types_offset);
  uint8_t call_encoding = *ptr++;
  uint16_t call_table_length;
  ptr += uleb(ptr, &call_table_length);
  if ((type_encoding != DW_EH_PE_absptr && type_encoding != DW_EH_PE_omit) ||
      call_encoding != DW_EH_PE_uleb128) {
    dbg("unknown encodings found: type: %d, call: %d\n", type_encoding,
        call_encoding);
    __fae_terminate();
  }
  prog_byte *end = ptr + call_table_length;
  uint16_t lp_ip;
  uint16_t action_offset;
  while (ptr < end) {
    uint16_t ip_start;
    ptr += uleb(ptr, &ip_start);
    uint16_t ip_range;
    ptr += uleb(ptr, &ip_range);
    ptr += uleb(ptr, &lp_ip);
    ptr += uleb(ptr, &action_offset);
    dbg("checking handler [0x%x, 0x%x], pc_offset: 0x%x\n", ip_start,
        ip_start + ip_range, pc_offset);
    if (ip_start < pc_offset && pc_offset <= ip_start + ip_range) {
      *lp_out = lp_ip;
      dbg("found handler [0x%x, 0x%x], lp: 0x%x\n", ip_start,
          ip_start + ip_range, *lp_out);
      goto found_handler;
    }
  }
  dbg("no suitible handler found\n");
  __fae_terminate();
found_handler:
  if (action_offset == 0) {
    dbg("no action\n");
    return 0;
  }
  ptr = end + action_offset - 1;
  while (true) {
    int16_t action, offset;
    ptr += sleb(ptr, &action);
    sleb(ptr, &offset);
    ptr += offset;
    dbg("checking entry %d\n", action);
    if (action != 0) {
      void *catch_type = type_table[-1 * action];
      if (catch_type == NULL) // if it is a catch(...) block, match anything
        return action;
      void *ptr = __fae_get_adjusted_exc(exc, catch_type);
      if (ptr) {
        return action;
      }
    } else {
      return 0;
    }
    if (offset == 0)
      break;
  }
  dbg("no action found\n");
  __fae_terminate();
}

// returns data pointer for pc entry. If no entry is found, return 0
table_data __fae_get_ptr(void *except, uint16_t pc) {
  table_data result;
  result.type_index = 0xff;
  pc <<= 1; // program counters are word-addressed
  dbg("unwinding at 0x%x with except 0x%x\n", pc, (uint16_t)except);
  const table_t __flash *table = (const table_t __flash *)__fae_table_start;
  unsigned length = table->length / sizeof(struct table_entry_t);
  for (int i = 0; i < length; i++) {
    if (table->data[i].pc_begin < pc && pc < table->data[i].pc_end) {
      result.data = table->data[i].data;
      result.data_end = table->data[i].data + table->data[i].length;
      if (table->data[i].lsda != NULL) {
        dbg("lsda found at 0x%x\n", (uint16_t)table->data[i].lsda);
        result.type_index =
            personality(table->data[i].lsda, pc - table->data[i].pc_begin,
                        except, &result.landing_pad);
        if (result.landing_pad == 0) {
          result.type_index = 0xff;
          return result;
        }
        dbg("type index 0x%x\n", result.type_index);
        dbg("landing offset: 0x%x\n", result.landing_pad);
        result.landing_pad += table->data[i].pc_begin;
        dbg("landing pad: 0x%x\n", result.landing_pad);
        result.landing_pad >>= 1;
      }
      dbg("found entry\n");
      return result;
    }
  }
  dbg("no entry found\n");
  result.data = 0;
  return result;
}