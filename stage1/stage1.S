#include <architecture/mipsregs.h>
#include <architecture/cp0.h>
#include <architecture/cache.h>
#include <kernel/ci20board.h>

/* USB Bootloader */

.set noreorder

.globl _start
/* Tell binutils it's a function */
.ent _start
.text

_start:

	/* Enable caching in kseg0 */
	li      t0, CACHE_MODE_CACHABLE_NONCOHERENT
	mtc0    t0, CP0_CONFIG
	nop

	/* Clear BSS */
	la      t0, __bss_start
	la      t1, __bss_end - 4
1:
	sw      zero, 0(t0)
	blt     t0, t1, 1b
	addiu   t0, t0, 4    

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
	sw ra, -64(sp)

	addi sp, sp, -64

	jal   entrypoint
	nop

	addi sp, sp, 64
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
	lw ra, -64(sp)

	move sp, t0

	jr ra
	nop

.end _start

