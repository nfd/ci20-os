#include <kernel/driver/jz47xx-uart/jz47xx-uart.h>
#include <system/board.h>
#include <architecture/peekpoke.h>

#include <stage1/timer_basic.h>
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

