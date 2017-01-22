#include "architecture/mipsregs.h"

.section .start,"x"

.set noreorder

.globl _start
.ent _start

.set noreorder
_start:
	/* args: a0 = core number */

	/* Clear BSS (first core only) */
	bnez a0, bss_cleared
	nop
	la      t0, __bss_start
	la      t1, __bss_end - 4
1:
	sw      zero, 0(t0)
	blt     t0, t1, 1b
	addiu   t0, t0, 4    /* in delay slot */

bss_cleared:

	/* Set up a stack. Sigma0 gets one 4k stack per core. */
	li t0, 4096
	mul t0, t0, a0 
	addiu t0, t0, 4096 /* sp at the top of the stack */

	la sp, stack
	add sp, sp, t0

	/* Jump to C */
	la t0, entrypoint
	jr t0
	nop

.set reorder

.end _start

.set reorder

