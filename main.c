#include "peekpoke.h"
#include "ci20board.h"
#include "uart.h"
#include "timer.h"

void entrypoint(void)
{
	//timer_init();
	uart_init();

	uart_puts("Hello, world!\n");

	while(1)
		;
}


