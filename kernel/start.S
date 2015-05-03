#include "mipsregs.h"

/* Status: Exceptions in cached memory, enable coprocessor registers and all interrupts */
#define CP0_STATUS_INITIALISER CP0_STATUS_CU_ALL | CP0_STATUS_IM_ALL | CP0_STATUS_IE

/* Cause: Use special location (0x80000200) for IRQ exceptions */
#define CP0_CAUSE_INITIALISER 0x800000

/* Not having ".set noreorder" caused an entertaining bug in which my BSS-clearing routine
 * never ended. I had this:
 *    blt t0, t1, 1b    ; clear more data
 *    addiu t0, t0, 4   ; Increment counter in delay slot
 *
 * ... but the compiler converted it to this:
 * 
 *    blt t0, t1, 1b    ; clear more data
 *    nop
 *    addiu t0, t0, 4   ; in delay slot
 *
 * I spent a little while wondering why my kernel never booted!
*/

.set noreorder

.text

.org 0x180
_exc_asm:
	eret

.org 0x200
_irq_asm:
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

	jal libci20_interrupt
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

	eret

.org 0x1000

/* make it accessible outside */
.globl _start
/* Tell binutils it's a function */
.ent _start

_start:
	li      t0, CP0_STATUS_INITIALISER
	mtc0    t0, CP0_STATUS
	nop

	li      t0, CP0_CAUSE_INITIALISER
	mtc0    t0, CP0_CAUSE
	nop

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
	addiu   t0, t0, 4    /* in delay slot */

	/* Set up a stack at 256M-8M */
	li sp, 0x8f800000

	/* And jump to C */
	la t0, entrypoint
	jr t0
	nop

.end _start

