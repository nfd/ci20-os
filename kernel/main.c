#include "libci20/peekpoke.h"
#include "ci20board.h"
#include "libci20/uart.h"
#include "libci20/timer.h"
#include "libci20/init.h"
#include "inttypes.h"

volatile uint32_t counter;

void counter_incrementer(void)
{
	counter++;
}

void entrypoint(void)
{
	libci20_init_baremetal();
	ostimer_init();

	uart_puts("Hello, world!\r\n");

	ostimer_register_callback(counter_incrementer);

	for(int run = 0; run < 3; run++) {
		uint32_t prev = counter;

		for(volatile uint32_t i = 0; i < 0xfffffff; i++)
			; // 00000C3C ms, 0xc48 (but between 0xc45 and 0xc51) on linux

		uart_print("%x4\r\n", counter - prev);
	}

	while(1) {
		__asm__("wait");
	}
}


