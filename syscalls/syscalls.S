#include <architecture/mipsregs.h>

.globl syscall_putchar_debug

syscall_putchar_debug:
	/* a0 contains character to write */
	li v0, 1
	syscall
	nop
	jr ra
	nop

.ent syscall_putchar_debug
.end syscall_putchar_debug

