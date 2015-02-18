#include "peekpoke.h"
#include "ci20board.h"
#include "debug_uart.h"

void entrypoint(void)
{
	debug_uart_init();

	/* LED blue to say we've arrived */
	write_l(GPIO_F_CLEAR, GPIO_F_LED_PIN); /* Turn LED blue */

	debug_uart_puts("Hello, world!\n");

	while(1)
		;
}

