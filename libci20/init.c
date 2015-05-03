#include "libci20/init.h"
#include "libci20/uart.h"
#include "libci20/interrupts.h"

void libci20_init_baremetal(void)
{
	uart_init();
	intc_init();
}

