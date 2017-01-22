#include "mipsregs.h"

.globl kernel_putchar
.ent kernel_putchar
	; our character in a0
	li v0, 1	/* syscall number */
	syscall
	jr ra
	nop

.end kernel_putchar

