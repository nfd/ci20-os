#include "libci20/peekpoke.h"
#include "ci20board.h"
#include "libci20/uart.h"
#include "libci20/timer.h"

void entrypoint(void)
{
	//timer_init();
	uart_init();

	uart_puts("Hello, world!\n");

	while(1)
		;
}


