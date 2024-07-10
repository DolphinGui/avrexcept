#include <avr/io.h>
#include <avr/pgmspace.h>
#include <cstdint>
#include <stdexcept>
#include <stdio.h>
#include <vector>

extern "C" void *get_SP() noexcept;
// experimentally SP begins at 2295

struct Except {
  int i;
};

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
  throw std::runtime_error("stack scan throws");
}

void sink(void *i) { printf("addr: %u\n", (uint16_t)&i); }
struct Destructable {
  ~Destructable() { printf("dtor called\n"); }
};

[[gnu::noinline]] int16_t n() {
  auto n = get_SP();
  Destructable d;
  try {
    sink(n);
    std::vector<uint8_t> vec{1, 2, 3, 4, 5, 6, 7};
    for (uint8_t u : vec) {
      printf("u: %d\n", u);
    }
    stack_scan(static_cast<char *>(n));
  } catch (int i) {
    printf("caught integer %d\n", i);
  } catch (...) {
    puts("caught something");
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
  } catch (std::exception const &e) {
    printf("caught std::exception: %s\n", e.what());
  }
  printf("done with main\n");
}
