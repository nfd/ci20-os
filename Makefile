AS=mipsel-unknown-elf-as -mips32
CC=mipsel-unknown-elf-gcc -mips32
LD=mipsel-unknown-elf-ld
OBJCOPY=mipsel-unknown-elf-objcopy
CFLAGS=-Os

OS_OBJS=start.o debug_uart.o main.o timer.o
STAGE1_OBJS=stage1.o stage1_main.o debug_uart.o ci20board.o

os.bin: os.elf
bootloader.bin: bootloader.elf

os.elf: $(OS_OBJS)
	$(LD) -T linker.lds -o $@ $+

bootloader.elf: $(STAGE1_OBJS)
	$(LD) -T bootloader.lds -o $@ $+

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

%.o: %.[Sc]
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.elf *.bin

