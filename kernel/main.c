#include "ci20board.h"
#include "driver/jz47xx-uart/jz47xx-uart.h"
#include "driver/jz47xx-timer/jz47xx-timer.h"
#include "system/init.h"
#include "inttypes.h"
#include "ci20board.h"
#include "architecture/peekpoke.h"

volatile uint32_t counter = 0;

void counter_incrementer(void)
{
	//counter++;
}

void entrypoint(void)
{
	system_init();
	uart_puts("Hello, world!\r\n");

	// ostimer_register_callback(counter_incrementer);

	for(int run = 0; run < 3; run++) {
		for(volatile uint32_t i = 0; i < 0xfffffff; i++)
			; 

		uart_print("%x4\r\n", counter);
	}

	while(1) {
		__asm__("wait");
	}
}


/* Potentially misleading name: this is the entrypoint for cores OTHER THAN
 * CORE 0, which have been started by core 0 during the kernel initialisation
 * process. */
void multicore_entrypoint(void)
{
	uart_puts("Hello, multicore world!\r\n");

	while(1) {
		counter++;
	}

	/*
	while(1) {
		__asm__("wait");
	}
	*/
}

