#include "uart.h"
#include "ci20board.h"
#include "timer.h"
#include "pllclock.h"
#include "peekpoke.h"
#include "memtest.h"
#include "ddr.h"

void ci20board_init();

void entrypoint()
{
	uart_init();
	uart_puts("Hi there\r\n");
	
	ci20board_init();
	pllclock_init();
	os_timer_init();
	ddr_init();

	memtest();

	while(1) {
		uint64_t val = os_timer_getval();
		uint64_t secs_approx = ((val >> 10) * 179) >>19; /* Very approx. :) */
		uart_print("Uptime in seconds: %x8.\r\n", secs_approx);
		usleep(1000000);
	}
}

