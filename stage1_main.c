#include "debug_uart.h"
#include "ci20board.h"

void ci20board_init();

void entrypoint()
{
	ci20board_init();

	debug_uart_init();
	debug_uart_puts("Hi there\r\n");
}

