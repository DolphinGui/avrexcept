.PHONEY: all clean sim

all: test.hex dump.txt

CC=avr-gcc
CXX=avr-g++.sh
AS=avr-as

MCU=atmega328p

ASFLAGS = -mmcu=$(MCU)
CFLAGS = -mmcu=$(MCU)  -maccumulate-args -funwind-tables -Oz -g
CXXFLAGS = -mmcu=$(MCU)  -maccumulate-args -funwind-tables -Oz -I. -frtti -g
CPPFLAGS = -DNDEBUG
LDFLAGS = -Wl,-gc-sections -mmcu=$(MCU) -funwind-tables

clean:
	rm -f *.elf *.o *.hex dump.txt

test.hex: test.elf
	avr-objcopy -j .text -j .data -O ihex test.elf test.hex

dump.txt: test.elf
	avr-objdump -Cd test.elf > dump.txt

test.elf: test.o crtbegin.o table_print.o print.o get_SP.o unwind.o unwind_sup.o unwind_supcxx.o
	$(CXX) -T link.lds -o  $@ $(LDFLAGS) $^

sim: test.elf
	simavr test.elf -m atmega328p

gdb: test.elf
	simavr test.elf -m atmega328p -g