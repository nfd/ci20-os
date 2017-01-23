#include <architecture/mipsregs.h>
#include <architecture/cp0.h>
#include <architecture/cache.h>
#include <architecture/memory.h>
#include <kernel/soc/jz4780/jz47xx-cp0.h>
#include <kernel/soc/jz4780/jz47xx-cache.h>
#include <architecture/kernelstack.h>

.set noreorder

/* Entry point for cores > 0. This is jumped to from stub code at the entry point. */

/* The jz4780 multicore entrypoint must be aligned to 64k. We do this by copying the stub, below, into TCSM. */

.macro jz4780_get_core_number dest
	mfc0 \dest, CP0_EBASE
	nop
	andi \dest, \dest, 3	/* CORE ID in lower two bits of EBASE */
.endm

/* This is copied (see multicore.c) to a 64k aligned location, so the only
 * absolute reference in it should be to the C entrypoint */
.globl multicore_entrypoint_asm
.ent multicore_entrypoint_asm
multicore_entrypoint_asm:
	jal _common_start
	nop

	li a0, JZ4780_CACHE_SIZE
	li a1, JZ4780_CACHE_LINE_SIZE

	jal mips_init_icache
	nop

	jal mips_init_dcache
	nop

	/* EBASE at 0x80000000 */
	mtc0 zero, CP0_EBASE
	nop

	/* Invalidate BTB */
	mfc0 t0, CP0_CONFIG7
	ori t0, t0, CP0_CONFIG7_BTB_INVALIDATE 
	mtc0 t0, CP0_CONFIG7
	nop

	/* Initialise the TLB by invalidating all the pages. */
.set reorder
	li t1, JZ4780_NUM_TLB_ENTRIES
	li t0, 0xa0000000
	mtc0 zero, CP0_ENTRYLO0
	mtc0 zero, CP0_ENTRYLO1
1:
	subu t1, 1
	mtc0 t0, CP0_ENTRYHI
	mtc0 t1, CP0_INDEX
	addiu t0, 0x2000		/* Two pages at a time */
	tlbwi
	bnez t1, 1b
.set noreorder

	/* Set up a stack for this core */
	jz4780_get_core_number a1
	mul t0, a1, 4096
	addiu t0, t0, 4096

	la sp, kernel_stack
	add sp, sp, t0

	/* ... and jump straight to C! */
	/* Why not jal here? Because you can only specify the low 28 bits of
	 * address with jal -- the upper 4 bits are retained, which means if our PC
	 * is currently in kseg1 (and it is) the dest address will also be in
	 * kseg1. */
	la t0, common_entrypoint
	jr t0
	nop
1:
	wait 	/* Should never get here */
	j 1b
	nop

.end multicore_entrypoint_asm

multicore_entrypoint_asm_end:

.data
.globl multicore_entrypoint_asm_length
multicore_entrypoint_asm_length:
	.word (multicore_entrypoint_asm_end - multicore_entrypoint_asm)


