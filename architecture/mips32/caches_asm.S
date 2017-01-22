#include <architecture/mipsregs.h>
#include <architecture/cp0.h>
#include <architecture/cache.h>
#include <architecture/memory.h>

#define CACHE_BASE_ADDR 0x80000000

.set noreorder

/* params a0 = cache size, a1 = cache line size */
.globl mips_init_icache
.ent mips_init_icache
mips_init_icache:
	/* Invalidate caches. */
	mtc0 zero, CP0_TAG_LO 
	ehb

	li t0, CACHE_BASE_ADDR
	addu t1, t0, a0
	/* ... icache: invalidate then load */
1:
	cache CACHE_R4000_INDEX_STORE_TAG_I, 0(t0)
	cache CACHE_R4000_FILL_I, 0(t0)
	blt t0, t1, 1b
	addu t0, t0, a1	/* delay slot */

	jr ra
	nop

.end mips_init_icache

/* params a0 = cache size, a1 = cache line size */
.globl mips_init_dcache
.ent mips_init_dcache
mips_init_dcache:
	/* ... dcache: invalidate then load */
	mtc0 zero, CP0_TAG_LO 
	ehb

	li t0, CACHE_BASE_ADDR
	addu t1, t0, a0
1:
	cache CACHE_R4000_INDEX_STORE_TAG_D, 0(t0)
	blt t0, t1, 1b
	addu t0, t0, a1	/* delay slot */

	li t0, CACHE_BASE_ADDR
1:
	lw t2, 0(t0)
	blt t0, t1, 1b
	addu t0, t0, a1	/* delay slot */

	jr ra
	nop

.end mips_init_dcache
