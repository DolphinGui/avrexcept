.PHONEY: all clean sim

all: test.hex dump.txt

CC=avr-gcc
CXX=avr-g++.sh
AS=avr-as

MCU=atmega328p

ASFLAGS = -mmcu=$(MCU)
CFLAGS = -mmcu=$(MCU)  -maccumulate-args -funwind-tables -Oz -falign-functions=16
CPPFLAGS = -mmcu=$(MCU)  -maccumulate-args -funwind-tables -Oz -I. -frtti -falign-functions=16
LDFLAGS = -Wl,-gc-sections -mmcu=$(MCU) -funwind-tables -falign-functions=16 -T

clean:
	rm -f *.elf *.o *.hex dump.txt

test.hex: test.elf
	avr-objcopy -j .text -j .data -O ihex test.elf test.hex

dump.txt: test.elf
	avr-objdump -Cd test.elf > dump.txt

test.elf: link.lds test.o crtbegin.o table_print.o print.o get_SP.o unwind.o unwind_sup.o unwind_supcxx.o
	avr-g++ -o $@ $(LDFLAGS) $^
	faegen $@
	avr-g++ -o $@ $(LDFLAGS) $^ __fae_data.o

sim: test.elf
	simavr test.elf -m atmega328p
