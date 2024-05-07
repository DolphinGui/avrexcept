#include <avr/io.h>
#include <avr/pgmspace.h>
#include <cstdint>
#include <stdio.h>

extern "C" void *get_SP() noexcept;
extern "C" void __fae_unwind() noexcept;

// experimentally SP begins at 2295
void stack_scan(const char *end) {
  int col = 0;
  for (auto ptr = reinterpret_cast<const char *>(2295); ptr != end; --ptr) {
    uint8_t c = *ptr;
    printf("%#x ", c);
    if (col++ == 4) {
      col = 0;
      puts("");
    }
  }
  puts("");
  throw 1;
}

void sink(int16_t &i) { printf("addr: %u\n", (uint16_t)&i); }

[[gnu::noinline]] int16_t n() {
  auto n = (int16_t)get_SP();
  sink(n);
  __fae_unwind();
  printf("after unwind\n");
  return n;
}

extern "C" void print_table();

int main() {
  printf("main!\n");
  auto s = n();
  printf("done with main\n");
  throw 12;
}
