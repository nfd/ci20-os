#include "memtest.h"
#include "inttypes.h"
#include "timer.h"
#include "uart.h"

#define MEM_START 0xA0000000
#define MEM_END   0xB0000000

void memtest(void)
{
	unsigned i;
	uint32_t expected = 0, got = 0;

	uart_print("Starting memory test...");

	for(i = MEM_START; i < MEM_END; i += (sizeof(uint32_t))) {
		uint32_t *ptr = (uint32_t *)i;

		if(i % 8)
			*ptr = 0x55555555;
		else
			*ptr = 0xAAAAAAAA;

		if(i % (1 << 20)) {
			uart_putc('.');
		}
	}

	for(i = MEM_START; i < MEM_END; i += (sizeof(uint32_t))) {
		got = *((uint32_t *)i);

		if(i % 8) {
			expected = 0x55555555;
		} else {
			expected = 0xAAAAAAAA;
		}

		if(expected != got) {
			uart_print("Location %x4, expected %x4, got %x4\r\n", i, expected, got);
			break;
		}
	}

	if(expected != got) {
		uart_print("Memory test failed.\r\n");
	} else {
		uart_print("Memory test passed.\r\n");
	}
}

