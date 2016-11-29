#include "ci20board.h"
#include "driver/jz47xx-uart/jz47xx-uart.h"
#include "driver/jz47xx-timer/jz47xx-timer.h"
#include "system/init.h"
#include "inttypes.h"

volatile uint32_t counter;

void counter_incrementer(void)
{
	counter++;
}

void entrypoint(void)
{
	system_init();

	// TODO move to system init?
	ostimer_init();

	uart_puts("Hello, world!\r\n");

	ostimer_register_callback(counter_incrementer);

	for(int run = 0; run < 3; run++) {
		uint32_t prev = counter;

		for(volatile uint32_t i = 0; i < 0xfffffff; i++)
			; 

		uart_print("%x4\r\n", counter - prev);
	}

	while(1) {
		__asm__("wait");
	}
}


