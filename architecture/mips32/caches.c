#include <inttypes.h>
#include <stringify.h>
#include <architecture/cache.h>
#include <kernel/soc/jz4780/jz47xx-cache.h>

void cache_writeback_range_d(void *start, int count)
{
	uint32_t idx = (uint32_t)start;
	uint32_t end = idx + count;

	for(; idx < end; idx += JZ4780_CACHE_LINE_SIZE) {
		__asm("cache " XSTRING(CACHE_R4000_HIT_WRITEBACK | CACHE_R4000_DCACHE) ", 0(%[a0]);" : : [a0] "r"(idx));
		__asm("cache " XSTRING(CACHE_R4000_HIT_WRITEBACK | CACHE_R4000_SCACHE) ", 0(%[a0]);" : : [a0] "r"(idx));
	}
}

void cache_synci_range(void *start, int count)
{
	uint32_t idx = (uint32_t)start;
	uint32_t end = idx + count;

	for(; idx < end; idx += JZ4780_CACHE_LINE_SIZE) {
		__asm("synci 0(%0);" : : "r"(idx));
	}
}

