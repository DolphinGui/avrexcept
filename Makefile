.PHONEY: all clean sim install echo

all: build/$(ARCH)/libfae.a

CC:=avr-gcc
CXX:=avr-g++.sh
AS:=avr-as

MCU:=atmega328p

C_SRC:=unwind_sup.c
CXX_SRC:=unwind_supcxx.cpp
ASM_SRC:=unwind.s

ASFLAGS:= --mlink-relax
CFLAGS :=  -maccumulate-args -funwind-tables -Oz -g
CXXFLAGS := -maccumulate-args -funwind-tables -Oz -I. -frtti -g
CPPFLAGS := -DNDEBUG

clean:
	rm -f *.elf *.o *.hex *.a dump.txt
	rm -rdf build

build/$(ARCH):
	mkdir -p $@

build/$(ARCH)/%.o: %.c | build/$(ARCH)
	$(CC)  -mmcu=$(ARCH) $(CPPFLAGS) $(CFLAGS) -c -o $@ $^

build/$(ARCH)/%.o: %.cpp | build/$(ARCH)
	$(CXX)  -mmcu=$(ARCH) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $^

build/$(ARCH)/%.o: %.s | build/$(ARCH)
	$(AS)  -mmcu=$(ARCH) $(ASFLAGS) -o $@ $^

build/$(ARCH)/libfae.a: build/$(ARCH)/unwind.o build/$(ARCH)/unwind_sup.o build/$(ARCH)/unwind_supcxx.o | build/$(ARCH)
	avr-ar -crs $@ $^

install: build/$(ARCH)/libfae.a
	install $^ $(PREFIX)/lib/$(ARCH)/libfae.a