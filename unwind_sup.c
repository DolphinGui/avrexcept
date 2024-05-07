#include "fae.h"
#include "stdint.h"
#include <avr/pgmspace.h>

typedef const uint8_t __flash prog_byte;
extern prog_byte __fae_table_start[];
extern prog_byte __fae_table_stop[];

typedef struct table_data {
  uint16_t data;     // r18-19
  uint16_t data_end; // r20-21
  uint16_t lsda;     // r22-23
} table_data;

// returns data pointer for pc entry. If no entry is found, return 0
table_data __fae_get_ptr(uint16_t pc) {
  table_data result;
  pc <<= 1;
  const table_t __flash *table = (const table_t __flash *)__fae_table_start;
  unsigned length = table->length / sizeof(struct table_entry_t);
  for (int i = 0; i < length; i++) {
    if (table->data[i].pc_begin < pc && pc < table->data[i].pc_end) {
      result.data = table->data[i].data;
      result.data_end = table->data[i].data + table->data[i].length;
      result.lsda = 0;
      return result;
    }
  }
  result.data = 0;
  return result;
}
