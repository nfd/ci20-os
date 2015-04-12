#include "mipsregs.h"
#include "ci20board.h"

/* USB Bootloader */

.globl _start
/* Tell binutils it's a function */
.ent _start
.text

_start:
	/* Enable caching in kseg0 */
	li      t0, CACHE_MODE_CACHABLE_NONCOHERENT
	mtc0    t0, CP0_CONFIG
	nop

	move t0, sp

	li  sp, 0xf4004000

	sw v0, -4(sp)
	sw v1, -8(sp)
	sw s0, -12(sp)
	sw s1, -16(sp)
	sw s2, -20(sp)
	sw s3, -24(sp)
	sw s4, -28(sp)
	sw s5, -32(sp)
	sw s6, -36(sp)
	sw s7, -40(sp)
	sw k0, -44(sp)
	sw k1, -48(sp)
	sw fp, -52(sp)
	sw gp, -56(sp)
	sw t0, -60(sp) /* sp */
	sw fp, -64(sp)
	sw ra, -68(sp)

	addi sp, sp, -68

	jal   entrypoint
	nop

	addi sp, sp, 68
	lw v0, -4(sp)
	lw v1, -8(sp)
	lw s0, -12(sp)
	lw s1, -16(sp)
	lw s2, -20(sp)
	lw s3, -24(sp)
	lw s4, -28(sp)
	lw s5, -32(sp)
	lw s6, -36(sp)
	lw s7, -40(sp)
	lw k0, -44(sp)
	lw k1, -48(sp)
	lw fp, -52(sp)
	lw gp, -56(sp)
	lw t0, -60(sp) /* sp */
	lw fp, -64(sp)
	lw ra, -68(sp)

	move sp, t0
	jr ra

.end _start

