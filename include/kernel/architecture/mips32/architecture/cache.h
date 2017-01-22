#define CACHE_MODE_UNCACHED 2
#define CACHE_MODE_CACHABLE_NONCOHERENT 3
#define CACHE_MODE_CACHABLE_COHERENT_EXCLUSIVE 4
#define CACHE_MODE_CACHABLE_COHERENT_EXCLUSIVE_WRITE 5

#define CACHE_R4000_ICACHE 0
#define CACHE_R4000_DCACHE 1
#define CACHE_R4000_SCACHE 3 /* Secondary cache */

#define	CACHE_R4000_INDEX_STORE_TAG_I 0x8
#define	CACHE_R4000_INDEX_STORE_TAG_D 0x9
#define CACHE_R4000_HIT_INVALIDATE 0x10
#define CACHE_R4000_FILL_I 0x14
#define CACHE_R4000_HIT_WRITEBACK 0x18

#ifndef __ASSEMBLER__
void cache_writeback_range_d(void *start, int count);
void cache_synci_range(void *start, int count);
#endif
