#include "inttypes.h"
#include "architecture/peekpoke.h"

#include "driver/jz47xx-uart/jz47xx-uart.h"
#include "driver/jz47xx-timer/jz47xx-timer.h"

#include "soc/jz4780/jz47xx-interrupts.h"

void jz4780_enable_core1();

void soc_jz4780_init()
{
	/* TODO rename uart_*(), intc_*(), and ostimer_*() to soc-specific names */
	uart_init();

	intc_init();

	// ostimer_init();

	jz4780_enable_core1();
}

