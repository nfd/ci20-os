#define ASM
#include "architecture/mipsregs.h"
#include "soc/jz4780/jz47xx-cp0.h"
#include "soc/jz4780/jz47xx-cache.h"
#include "architecture/kernelstack.h"

.set noreorder

/* Entry point for cores > 0. This is jumped to from stub code at the entry point. */

/* The jz4780 multicore entrypoint must be aligned to 64k. The easiest option
 * is to use .balign, but this will waste space as it just increments the
 * instruction pointer to the next aligned region. Instead, we add a multicore
 * stub in kernel/start.S which jumps to this function -- a little messier, but
 * doesn't add (on average) 32KiB of zeroes.
*/
/* .balign 0x10000 */

.globl multicore_entrypoint_asm
.ent multicore_entrypoint_asm
multicore_entrypoint_asm:
	/* Cache init: invalidate all cache lines */
	li      t0, KSEG0_START
	ori     t1, t0, JZ4780_CACHE_SIZE
	mtc0    zero, CP0_TAG_LO
	mtc0    zero, CP0_TAG_HI
	ehb
1:	cache   CACHE_R4000_INDEX_STORE_TAG | CACHE_R4000_ICACHE, 0(t0)
	cache   CACHE_R4000_INDEX_STORE_TAG | CACHE_R4000_DCACHE, 0(t0)
	bne     t0, t1, 1b
	addiu   t0, t0, JZ4780_CACHE_LINE_SIZE

	mtc0    zero, CP0_TLB_PAGEMASK
	ehb

	/* Enable caching in kseg0 */
	li      t0, CACHE_MODE_CACHABLE_NONCOHERENT
	mtc0    t0, CP0_CONFIG
	ehb

	/* Set up a stack for this core -- TODO which we assume is the first one. */
	la sp, kernel_stack
	addiu sp, sp, KERNEL_STACK_SIZE   /* Top of stack for core 0 */
	addiu sp, sp, KERNEL_STACK_SIZE   /* Top of stack for core 1 */

	/* ... and jump straight to C! */
	la t0, multicore_entrypoint
	jr t0
	nop

.end multicore_entrypoint_asm

