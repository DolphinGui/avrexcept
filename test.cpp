#include <alloca.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <cstdint>
#include <stdio.h>

extern "C" void *get_SP() noexcept;
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

void sink(void *i) {
  char *n = static_cast<char *>(alloca((uint16_t)i / 4));
  printf("addr: %u, %u\n", (uint16_t)&i, (uint16_t)n);
}
struct Except {};

struct Destructable {
  ~Destructable() { printf("dtor called\n"); }
};

[[gnu::noinline]] int16_t n() {
  auto n = get_SP();
  Destructable d;
  try {
    sink(n);
    throw 12;
    stack_scan(static_cast<char *>(n));
  } catch (int i) {
    printf("rethrowing integer %d\n", i);
    throw;
  }
  printf("after unwind\n");
  return 12;
}

extern "C" void print_table();

int main() {
  printf("main!\n");
  try {
    auto s = n();
  } catch (int i) {
    printf("caught integer %d\n", i);
  }
  printf("done with main\n");
}
