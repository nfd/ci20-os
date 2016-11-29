#include "memtest.h"
#include "inttypes.h"
#include "driver/jz47xx-uart/jz47xx-uart.h"
#include "driver/jz47xx-timer/jz47xx-timer.h"

#define MEM_START 0x80000000
#define MEM_END   (MEM_START + (32*1024*1024)) /* Just test a small part, for speed */

void memtest(void)
{
	unsigned i;
	uint32_t expected = 0, got = 0;
	uart_puts("Performing memory test...");

	for(i = MEM_START; i < MEM_END; i += (sizeof(uint32_t))) {
		volatile uint32_t *ptr = (uint32_t *)i;

		*ptr = i + 2;
	}

	for(i = MEM_START; i < MEM_END; i += (sizeof(uint32_t))) {
		volatile uint32_t *ptr = (uint32_t *)i;
		got = *ptr;

		expected = i + 2;

		if(expected != got) {
			break;
		}
	}

	if(expected != got) {
		uart_puts("FAILED!\r\n");
	} else {
		uart_puts("passed.\r\n");
	}
}

