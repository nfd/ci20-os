/* Very naive implementation */
#include "string.h"

#include "inttypes.h"

void * memset(void *b, int c, size_t len)
{
	uint8_t *pos = b, *end = (uint8_t *)b + len;

	for(; pos != end; pos++)
		*pos = c;

	return b;
}

