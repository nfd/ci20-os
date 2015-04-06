AS=mipsel-unknown-elf-as -mips32
CC=mipsel-unknown-elf-gcc
LD=mipsel-unknown-elf-ld
OBJCOPY=mipsel-unknown-elf-objcopy
CFLAGS=-mips32r2 -EL -Os -std=c99 -Wall -Werror
# TODO: also consider adding: -mno-fused-madd -msoft-float -ffreestanding -nostdlib -mno-abicalls

OS_OBJS=start.o uart.o main.o timer.o peekpoke.o
STAGE1_OBJS=stage1.o stage1_main.o uart.o ci20board.o timer.o peekpoke.o pllclock.o ddr.o memtest.o

os.bin: os.elf
bootloader.bin: bootloader.elf

os.elf: $(OS_OBJS)
	$(LD) -T linker.lds -o $@ $+

bootloader.elf: $(STAGE1_OBJS) bootloader.lds
	$(LD) -T bootloader.lds -o $@ $+

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

ddr.c: ddr.py
	python3 ddr.py >ddr.c

%.o: %.[Sc]
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.elf *.bin

