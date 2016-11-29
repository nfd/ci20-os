#include "driver/jz47xx-uart/jz47xx-uart.h"

#include "soc/jz4780/jz47xx-interrupts.h"

void soc_jz4780_init(void)
{
	uart_init();
	intc_init();
}

