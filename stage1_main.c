#include "uart.h"
#include "ci20board.h"
#include "timer.h"

void ci20board_init();

void entrypoint()
{
	ci20board_init();

	os_timer_init();

	uart_init();
	uart_puts("Hi there\r\n");

	while(1) {
		uart_print("Uptime in seconds * 3000000: %x8.\r\n", os_timer_getval());
		usleep(1000000);
	}
}

