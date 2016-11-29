#include "driver/jz47xx-uart/jz47xx-uart.h"
#include "ci20board.h"
#include "architecture/peekpoke.h"
#include "timer_basic.h"
#include "pllclock.h"
#include "ddr.h"
#include "memtest.h"

void entrypoint()
{
	uart_init();
	uart_puts("Hi there\r\n");
	
	pllclock_init();
	spintimer_init();
	ddr_init();

	memtest();
}

