#include "fae.h"
#include "stdint.h"
#include <avr/pgmspace.h>
#include <stdio.h>

extern prog_byte __lsda_begin[];
extern prog_byte __fae_table_start[];
extern prog_byte __fae_table_stop[];

void __fae_terminate() __attribute__((noreturn));

typedef struct table_data {
  prog_byte *data;     // r18-19
  prog_byte *data_end; // r20-21
  prog_byte *lsda;     // r22-23
  // uint16_t type_index; // r24-25
} table_data;

static uint8_t uleb(prog_byte *ptr, uint16_t *out) {
  *out = *ptr++;
  if (!(*out & 0b10000000)) {
    return 1;
  } else {
    *out &= 0b01111111;
    *out <<= 7;
    *out |= (*ptr & 0b01111111);
  }
  return 2;
}

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
// this is very bad, and probably should be rewritten in assembly for speed
// need to figure out a way to profile via simavr
static void print_lsda(prog_byte *ptr, uint16_t pc_offset) {
  typedef void *const __flash prog_ptr;
  uint8_t lp_encoding = *ptr++;
  uint16_t lp_offset = 0;
  if (lp_encoding != DW_EH_PE_omit) {
    ptr += uleb(ptr, &lp_offset);
  }
  uint8_t type_encoding = *ptr++;
  uint16_t types_offset;
  ptr += uleb(ptr, &types_offset);
  prog_ptr *type_table = (prog_ptr *)(ptr + types_offset);
  uint8_t call_encoding = *ptr++;
  uint16_t call_table_length;
  ptr += uleb(ptr, &call_table_length);
  printf("lp_encoding: 0x%x, type_encoding: 0x%x, type offset: 0x%x\ncall "
         "encoding: 0x%x, call length: 0x%x\n",
         lp_encoding, type_encoding, types_offset, call_encoding,
         call_table_length);
  if (type_encoding != DW_EH_PE_absptr || call_encoding != DW_EH_PE_uleb128) {
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
    if (ip_start < pc_offset && pc_offset <= ip_start + ip_range) {
      goto found_handler;
    }
  }
  __fae_terminate();
found_handler:
  if (action_offset == 0) {
    return; // properly handle this later
  }
  ptr = end + action_offset - 1;
  while (true) {
    int16_t action, offset;
    ptr += sleb(ptr, &action);
    sleb(ptr, &offset);
    ptr += offset;
    printf("action %i, offset %i ", action, offset);
    if (action != 0)
      print_typename(type_table[-1 * action]);
    if (offset == 0)
      break;
  }
}

void _Unwind_RaiseException() {
  printf("unwind_raise called\n");
  __fae_terminate();
}


// returns data pointer for pc entry. If no entry is found, return 0
table_data __fae_get_ptr(uint16_t pc) {
  table_data result;
  pc <<= 1; // program counters are word-addressed
  const table_t __flash *table = (const table_t __flash *)__fae_table_start;
  unsigned length = table->length / sizeof(struct table_entry_t);
  for (int i = 0; i < length; i++) {
    if (table->data[i].pc_begin < pc && pc < table->data[i].pc_end) {
      result.data = table->data[i].data;
      result.data_end = table->data[i].data + table->data[i].length;
      result.lsda = table->data[i].lsda;
      printf("data: 0x%x\n", (uint16_t)result.data);
      if (result.lsda != (prog_byte *)0xffff) {
        printf("lsda: 0x%x\n", (uint16_t)result.lsda);
        printf("lsda begin: 0x%x\n", (uint16_t)__lsda_begin);
        print_lsda(result.lsda, pc - table->data[i].pc_begin);
      }
      return result;
    }
  }
  result.data = 0;
  return result;
}