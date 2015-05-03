PREFIX=mipsel-unknown-elf-
AS=$(PREFIX)as -mips32
CC=$(PREFIX)gcc
LD=$(PREFIX)ld
AR=$(PREFIX)ar
OBJCOPY=$(PREFIX)objcopy
CFLAGS=-mips32r2 -EL -O2 -std=c99 -Wall -Werror -Iinclude -G0
# TODO: also consider adding: -mno-fused-madd -msoft-float -ffreestanding -nostdlib -mno-abicalls
#

obj = $(patsubst %,build/%,$(addsuffix .o,$(basename $(1))))

# main targets
.PHONY: os
os: build/stage1.elf build/kernel.elf

# 
# Bulk of OS functionality in here.
#
LIBCI20_SRC=libci20/uart.c libci20/peekpoke.S libci20/pllclock.c libci20/timer.c libci20/init.c libci20/interrupts.c

build/libci20.a: $(call obj,$(LIBCI20_SRC))
	$(AR) rcs $@ $+

# 
# Miniature version for stage1
#
LIBCI20MINI_SRC=libci20/uart.c libci20/peekpoke.S libci20/pllclock.c libci20/timer_basic.c libci20/init_basic.c
build/libci20mini.a: $(call obj,$(LIBCI20MINI_SRC))
	$(AR) rcs $@ $+

# 
# stage 1
#
STAGE1_SRC=stage1/stage1.S stage1/main.c stage1/ci20board.c stage1/memtest.c

build/stage1/ddr.o: build/stage1/ddr.c

build/stage1/ddr.c: stage1/ddr.py
	python3 stage1/ddr.py >build/stage1/ddr.c

build/stage1.elf: $(call obj,$(STAGE1_SRC)) build/stage1/ddr.o stage1/stage1.lds build/libci20mini.a
	$(LD) -T stage1/stage1.lds -o $@ $(call obj,$(STAGE1_SRC)) build/stage1/ddr.o build/libci20mini.a

#
# OS
#
KERNEL_SRC=kernel/start.S kernel/main.c

build/kernel.elf: $(call obj,$(KERNEL_SRC)) kernel/linker.lds build/libci20.a
	$(LD) -T kernel/linker.lds -o $@ $(call obj,$(KERNEL_SRC)) build/libci20.a

# A recipe to convert ELF to raw memory images. These are no longer required
# for USB loading.
%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

.SECONDEXPANSION:

# Directory secondexpansion magic thanks to http://www.cmcrossroads.com/article/making-directories-gnu-make?page=0%2C2
build/%.o: %.[Sc] $$(@D)/.f
	$(CC) $(CFLAGS) -c -o $@ $<

%/.f:
	mkdir -p $(dir $@) 
	touch $@

.PRECIOUS: %/.f


clean:
	rm -rf build

