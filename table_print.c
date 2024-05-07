#include "fae.h"
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdio.h>

typedef const uint8_t __flash prog_byte;

extern prog_byte __fae_table_start[];
extern prog_byte __fae_table_stop[];

static uint8_t denumerate(reg r) {
  switch (r) {
  case r2:
    return 2;
  case r3:
    return 3;
  case r4:
    return 4;
  case r5:
    return 5;
  case r6:
    return 6;
  case r7:
    return 7;
  case r8:
    return 8;
  case r9:
    return 9;
  case r10:
    return 10;
  case r11:
    return 11;
  case r12:
    return 12;
  case r13:
    return 13;
  case r14:
    return 14;
  case r15:
    return 15;
  case r16:
    return 16;
  case r17:
    return 17;
  case r28:
    return 28;
  case r29:
    return 29;
  default:
    assert(false);
  }
}

static void print_inst(frame_inst inst) {
  if (is_pop(inst)) {
    printf("pop r%i", denumerate(get_reg(inst.p)));
  } else {
    printf("skip %i bytes", inst.s.bytes);
  }
}

static void print_fae(prog_byte *start, prog_byte *stop) {
  printf("loc: %i\n", (uint16_t)start);
  const table_t __flash *table = (const table_t __flash *)start;
  for (int i = 0; i != 8; i++) {
    uint8_t c = table->header[i];
    putchar(c);
  }
  unsigned length = table->length / sizeof(struct table_entry_t);
  printf("\nentries: %i\n", length);
  for (uint16_t i = 0; i != length; i++) {
    printf("\npc: [%i %i]", table->data[i].pc_begin, table->data[i].pc_end);
    if (table->data[i].data != -1) {
      unsigned length = table->data[i].length;
      printf(" data: 0x%x, len: %i\n", table->data[i].data, length);
      unsigned begin = table->data[i].data;
      for (int i = 0; i < length; i++) {
        printf("p: 0x%x\n", pgm_read_byte(i + begin));
      }
    }
  }
  prog_byte *start_table = start + table->length;
  printf("table begin: 0x%x\n", (uint16_t)start_table);
  printf("table size: %i\n", stop - start_table);
}

static void print(const uint8_t __flash *start, const uint8_t __flash *stop) {
  int size = start - stop;
  int size2 = stop - start;
  printf("size: %i, size2: %i\n", size, size2);
  for (int i = 0; i != 128; i++) {
    printf("0x%x ", start[i]);
    if (i % 8 == 0) {
      puts("");
    }
  }
}

void print_table() {
  print_fae(__fae_table_start, __fae_table_stop);
  // print(__fae_table_start, __fae_table_stop);
}