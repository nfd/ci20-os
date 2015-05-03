#include "libci20/init.h"
#include "libci20/uart.h"

void libci20_init_baremetal(void)
{
	uart_init();
	// No interrupts to initialise
}

