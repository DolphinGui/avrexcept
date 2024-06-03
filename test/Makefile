.PHONEY: all clean sim

all: test.hex dump.txt

CC:=avr-gcc
CXX:=avr-g++.sh
AS:=avr-as

MCU:=atmega328p

ASFLAGS := -mmcu=$(MCU)
FLAGS :=   -maccumulate-args -funwind-tables -ffunction-sections  -mmcu=$(MCU) -Oz -g -I . -mcall-prologues
CFLAGS := $(FLAGS)
CXXFLAGS := $(FLAGS) -frtti
CPPFLAGS := -DNDEBUG
LDFLAGS := -Wl,-gc-sections -mmcu=$(MCU) -funwind-tables -lfae -Xlinker -Map=output.map 

clean:
	rm -f *.elf *.o *.hex dump.txt output.map

test.hex: test.elf
	avr-objcopy -j .text -j .data -O ihex test.elf test.hex

dump.txt: test.elf
	avr-objdump -Cd test.elf > dump.txt

test.elf: test.o print.o get_SP.o
	$(CXX) -o  $@ $^ $(LDFLAGS)

sim: test.elf
	simavr test.elf -m atmega328p

gdb: test.elf
	simavr test.elf -m atmega328p -g