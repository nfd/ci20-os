#include "string.h"

#include "inttypes.h"

void * memcpy(void *dst_v, void *src_v, size_t len)
{
	uint8_t *dst = dst_v;
	uint8_t *src = src_v;

	while(len--) {
		*dst++ = *src++;
	}

	return dst_v;
}

