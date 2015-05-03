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
	sw at, -4(sp)
	sw v0, -8(sp)
	sw v1, -12(sp)
	sw a0, -16(sp)
	sw a1, -20(sp)
	sw a2, -24(sp)
	sw a3, -28(sp)
	sw t0, -32(sp)
	sw t1, -36(sp)
	sw t2, -40(sp)
	sw t3, -44(sp)
	sw t4, -48(sp)
	sw t5, -52(sp)
	sw t6, -56(sp)
	sw t7, -60(sp)
	sw s0, -64(sp)
	sw s1, -68(sp)
	sw s2, -72(sp)
	sw s3, -76(sp)
	sw s4, -80(sp)
	sw s5, -84(sp)
	sw s6, -88(sp)
	sw s7, -92(sp)
	sw t8, -96(sp)
	sw t9, -100(sp)
	sw gp, -104(sp)
	sw fp, -108(sp)
	sw ra, -112(sp)

	addi sp, sp, -112

	jal libci20_interrupt
	nop

	addi sp, sp, 112
	lw at, -4(sp)
	lw v0, -8(sp)
	lw v1, -12(sp)
	lw a0, -16(sp)
	lw a1, -20(sp)
	lw a2, -24(sp)
	lw a3, -28(sp)
	lw t0, -32(sp)
	lw t1, -36(sp)
	lw t2, -40(sp)
	lw t3, -44(sp)
	lw t4, -48(sp)
	lw t5, -52(sp)
	lw t6, -56(sp)
	lw t7, -60(sp)
	lw s0, -64(sp)
	lw s1, -68(sp)
	lw s2, -72(sp)
	lw s3, -76(sp)
	lw s4, -80(sp)
	lw s5, -84(sp)
	lw s6, -88(sp)
	lw s7, -92(sp)
	lw t8, -96(sp)
	lw t9, -100(sp)
	lw gp, -104(sp)
	lw fp, -108(sp)
	lw ra, -112(sp)

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

