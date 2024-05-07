.PHONEY: all clean

all: test.hex dump.txt

CC=avr-gcc
CXX=avr-g++.sh
AS=avr-as

MCU=atmega328p

ASFLAGS = -mmcu=$(MCU)
CFLAGS = -mmcu=$(MCU)  -maccumulate-args -funwind-tables -Oz
CPPFLAGS = -mmcu=$(MCU)  -maccumulate-args -funwind-tables -Oz -I.
LDFLAGS = -Wl,-gc-sections -mmcu=$(MCU) -funwind-tables -T link.lds

clean:
	rm -f *.elf *.o *.hex dump.txt

test.hex: test.elf
	avr-objcopy -j .text -j .data -O ihex test.elf test.hex

dump.txt: test.elf
	avr-objdump -Cd test.elf > dump.txt

test.elf: test.o crtbegin.o table_print.o print.o get_SP.o unwind.o unwind_sup.o
	avr-g++.sh -o $@  $^ $(LDFLAGS)
	faemap $@
	avr-g++ -o $@ __fae_data.o $^ $(LDFLAGS) -Xlinker -Map=output.map
