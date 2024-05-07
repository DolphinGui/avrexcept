#pragma once

#include "assert.h"
#include "stdbool.h"
#include "stdint.h"

typedef struct info {
  uint32_t version;
  uint32_t length; // in bytes, not including this
  struct entry {
    uint32_t offset;
    uint32_t length;
    uint32_t begin;
    uint32_t begin_pc_symbol; // (index into symtab)
    uint32_t range;
    uint32_t range_pc_symbol;
    uint32_t lsda_symbol;
    uint32_t cfa_reg;
  } data[];
} info;

typedef struct table_entry_t {
  uint16_t pc_begin;
  uint16_t pc_end;
  uint16_t data;
  uint16_t length;
  uint16_t lsda;
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

inline reg enumerate(uint8_t r) {
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

inline reg get_reg(pop p) { return p._reg & ~0b10000000; }
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

/* When unwinding, first see if cfa_reg is nonzero. If so, use out to
   load cfa_reg to SP. Then, do pop and skip to restore register state.
   Finally, pop the return address and use that to look up the next unwind.*/