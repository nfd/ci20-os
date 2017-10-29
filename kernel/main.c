#include <kernel/driver/jz47xx-uart/jz47xx-uart.h>
#include <kernel/driver/jz47xx-timer/jz47xx-timer.h>
#include <kernel/system/init.h>
#include <architecture/userstate.h>

#include "sigma0_interface.h"

volatile uint32_t counter = 0;

void counter_incrementer(void)
{
	counter++;
}

struct architecture_userstate current;

/* Entrypoint for all cores after the system has started */
void common_entrypoint(void)
{
	uart_puts("Hello, world!");
	for(volatile int i = 0; i < 0xffffff; i++)
		;

	jump_to_sigma0();

	/* Should never get here */
	while(1) {
		__asm__("wait");
	}
}

/* Entrypoint for the first core (on system startup). */
void unicore_entrypoint(void)
{
	system_init();
	ostimer_register_callback(counter_incrementer);

	common_entrypoint();
}

