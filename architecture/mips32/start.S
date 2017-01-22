#include "architecture/mipsregs.h"
#include "architecture/cp0.h"
#include "architecture/cache.h"
#include "architecture/kernelstack.h"

.section .start,"x"

.set noreorder

.macro saveregisters
	sw _at, 0(k0)
	sw v0, 4(k0)
	sw v1, 8(k0)
	sw a0, 12(k0)
	sw a1, 16(k0)
	sw a2, 20(k0)
	sw a3, 24(k0)
	sw t0, 28(k0)
	sw t1, 32(k0)
	sw t2, 36(k0)
	sw t3, 40(k0)
	sw t4, 44(k0)
	sw t5, 48(k0)
	sw t6, 52(k0)
	sw t7, 56(k0)
	sw s0, 60(k0)
	sw s1, 64(k0)
	sw s2, 68(k0)
	sw s3, 72(k0)
	sw s4, 76(k0)
	sw s5, 80(k0)
	sw s6, 84(k0)
	sw s7, 88(k0)
	sw t8, 92(k0)
	sw t9, 96(k0)
	sw gp, 100(k0)
	sw fp, 104(k0)
	sw ra, 108(k0)
	sw sp, 112(k0)

.endm

.macro saveregisters_exc
	mfc0 t0, CP0_EPC
	sw t0, 116(k0)
.endm

.macro restoreregisters
	lw _at, 0(k0)
	lw v0, 4(k0)
	lw v1, 8(k0)
	lw a0, 12(k0)
	lw a1, 16(k0)
	lw a2, 20(k0)
	lw a3, 24(k0)
	lw t0, 28(k0)
	lw t1, 32(k0)
	lw t2, 36(k0)
	lw t3, 40(k0)
	lw t4, 44(k0)
	lw t5, 48(k0)
	lw t6, 52(k0)
	lw t7, 56(k0)
	lw s0, 60(k0)
	lw s1, 64(k0)
	lw s2, 68(k0)
	lw s3, 72(k0)
	lw s4, 76(k0)
	lw s5, 80(k0)
	lw s6, 84(k0)
	lw s7, 88(k0)
	lw t8, 92(k0)
	lw t9, 96(k0)
	lw gp, 100(k0)
	lw fp, 104(k0)
	lw ra, 108(k0)
	lw sp, 112(k0)
.endm

.macro restoreregisters_exc
	lw t0, 116(k0)
	mtc0 t0, CP0_EPC
.endm

/* We use ... 01 1111 i.e. cache=3, dirty=1, valid=1, global=1 */
#define ENTRYLO_DIRECT_MAP_TEMPLATE 0x1f

.org 0x0
.set noreorder
.set noat
_exc_tlb_refill:
	/* Directly map userspace to page frames */
	mfc0 k0, CP0_BADVADDR

	/* k0 will become entrylo -- mask off everything below the frame */
	srl k0, k0, 12
	move k1, k0

	/* ... move into the right place for entrylo. */
	sll k0, k0, 6
	/* ... and mask in our default ENTRYLO setup */
	ori k0, k0, ENTRYLO_DIRECT_MAP_TEMPLATE

	mtc0 k0, CP0_ENTRYLO0
	/* Prepare entrylo1 */
	addi k1, k1, 1 /* next page */
	sll k1, k1, 6
	ori k1, k1, ENTRYLO_DIRECT_MAP_TEMPLATE
	mtc0 k1, CP0_ENTRYLO1
	nop

	tlbwr
	nop

	eret
	nop

.org 0x180
_exc_asm:
	j _exc_asm_continue
	nop

.org 0x200
_irq_asm:
	la k0, current
	saveregisters

	la sp, kernel_stack
	jal libci20_interrupt
	addi sp, sp, KERNEL_STACK_SIZE - 4 /* in delay slot */

	la k0, current
	restoreregisters

	eret
	nop

_exc_asm_continue:
	la k0, current
	saveregisters
	saveregisters_exc

	la sp, kernel_stack

	addi sp, sp, KERNEL_STACK_SIZE - 4
	mfc0 a3, CP0_CAUSE

	jal mips_exception
	nop

	la k0, current
	restoreregisters
	restoreregisters_exc

	eret
	nop

.set at

/* Initialisation common to both initial core and multicore, called from unicore and multicore entrypoints */
.globl _common_start
.ent _common_start
_common_start:
	mfc0    t0, CP0_STATUS

	/* Preserve SR (only) */
	and     t0, CP0_STATUS_SR

	/* Enable all interrupts and coprocessors -- TODO! */
	or      t0, (CP0_STATUS_IE | CP0_STATUS_IM_ALL | CP0_STATUS_CU_ALL)

	mtc0    t0, CP0_STATUS
	ehb

	/* Use 80000200 for IRQ exceptions */
	li      t0, CP0_CAUSE_IV
	mtc0    t0, CP0_CAUSE
	ehb

	/* Enable caching in kseg0 */
	li      t0, CACHE_MODE_CACHABLE_NONCOHERENT
	mtc0    t0, CP0_CONFIG
	nop

	/* 4k pages */
	mtc0    zero, CP0_TLB_PAGEMASK
	ehb

	jr ra
	nop
.end _common_start

.globl _unicore_entrypoint_asm
.ent _unicore_entrypoint_asm

_unicore_entrypoint_asm:
	jal _common_start
	nop

	/* Clear BSS */
	la      t0, __bss_start
	la      t1, __bss_end - 4
1:
	sw      zero, 0(t0)
	blt     t0, t1, 1b
	addiu   t0, t0, 4    /* in delay slot */

	/* Set up a stack for the first core. */
	la sp, kernel_stack
	addiu sp, sp, KERNEL_STACK_SIZE

	/* Jump to C */
	la t0, unicore_entrypoint
	jr t0
	nop
1:
	wait /* should never get here */
	j 1b
	nop

.end _unicore_entrypoint_asm

.set reorder

