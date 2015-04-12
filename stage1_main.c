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

	usleep(100000);

}

