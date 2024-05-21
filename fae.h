#pragma once

#include "assert.h"
#include "stdbool.h"
#include "stdint.h"

typedef const uint8_t __flash prog_byte;

typedef struct table_entry_t {
  uint16_t pc_begin;
  uint16_t pc_end;
  prog_byte *data;
  uint8_t frame_reg;
  uint8_t length;
  prog_byte *lsda;
} table_entry;

typedef struct table_t {
  char header[8];
  uint16_t length;
  table_entry data[];
} table_t;
/* Entries are aligned by 2 so that personality_ptr
   can be read by a single movw instruction. Pad
    using 0x00. Entries may be null terminated to
    simplify debugging */

// 0x00 is padding byte, should only occur at the end of the entry.
// high bit is 0
typedef struct skip {
  uint8_t bytes;
} skip;
// skip make_skip(uint8_t b){return skip{ b & ~0b10000000};}

typedef enum reg : uint8_t {
  r2,
  r3,
  r4,
  r5,
  r6,
  r7,
  r8,
  r9,
  r10,
  r11,
  r12,
  r13,
  r14,
  r15,
  r16,
  r17,
  r28,
  r29
} reg;

inline uint8_t enumerate(uint8_t r) {
  assert(r < 2);
  assert(r > 17 && r < 28);
  assert(r > 29);
  if (r < 18) {
    return r - 2;
  } else {
    return r - 12;
  }
}

// high bit is 1
typedef struct pop {
  uint8_t _reg;
} pop;

inline uint8_t get_reg(pop p) { return p._reg & ~0b10000000; }
inline pop make_pop(reg r) {
  pop result;
  result._reg = r | 0b10000000;
  return result;
}

typedef union frame_inst {
  pop p;
  skip s;
  uint8_t byte;
} frame_inst;
inline bool is_pop(frame_inst f) { return f.byte & 0b10000000; }
inline bool is_skip(frame_inst f) { return !is_pop(f); }

enum {
  DW_EH_PE_absptr = 0x00,
  DW_EH_PE_omit = 0xff,

  /* FDE data encoding.  */
  DW_EH_PE_uleb128 = 0x01,
  DW_EH_PE_udata2 = 0x02,
  DW_EH_PE_udata4 = 0x03,
  DW_EH_PE_udata8 = 0x04,
  DW_EH_PE_sleb128 = 0x09,
  DW_EH_PE_sdata2 = 0x0a,
  DW_EH_PE_sdata4 = 0x0b,
  DW_EH_PE_sdata8 = 0x0c,
  DW_EH_PE_signed = 0x08,

  /* FDE flags.  */
  DW_EH_PE_pcrel = 0x10,
  DW_EH_PE_textrel = 0x20,
  DW_EH_PE_datarel = 0x30,
  DW_EH_PE_funcrel = 0x40,
  DW_EH_PE_aligned = 0x50,

  DW_EH_PE_indirect = 0x80
};
