#include "libci20/uart.h"
#include "ci20board.h"
#include "libci20/timer.h"
#include "libci20/pllclock.h"
#include "libci20/peekpoke.h"
#include "libci20/init.h"
#include "memtest.h"
#include "ddr.h"

void ci20board_init();

void entrypoint()
{
	libci20_init_baremetal();
	uart_puts("Hi there\r\n");
	
	ci20board_init();
	pllclock_init();
	os_timer_init();
	ddr_init();

	memtest();

	usleep(100000);

}

