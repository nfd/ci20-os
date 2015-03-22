#include "uart.h"
#include "ci20board.h"
#include "timer.h"
#include "pllclock.h"
#include "peekpoke.h"
#include "memtest.h"

void ci20board_init();

void entrypoint()
{
	ci20board_init();
	os_timer_init();
	pllclock_init();
	ddr_init();

	uart_init();
	uart_puts("Hi there\r\n");
	
	memtest();

	while(1) {
		uart_print("Uptime in seconds * 3000000: %x8.\r\n", os_timer_getval());
		usleep(1000000);
	}
}

