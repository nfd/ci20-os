#include "inttypes.h"
#include "driver/jz47xx-timer/jz47xx-timer.h"
#include "driver/jz47xx-uart/jz47xx-uart.h"
#include "architecture/peekpoke.h"
#include "soc/jz4780/jz47xx-interrupts.h"
#include "ci20board.h"
#include "timer_internal.h"

#define MAX_TIMER_CALLBACKS 4

/*
 * OS timer support
*/
static int timer_callback_count;
static timer_callback timer_callbacks[4];

void ostimer_init(void)
{
	/* Timer wraps when it hits comparison value */
	poke32(TCU_OSTCSR, OSTCSR_PRESCALE_16);

	/* Counter initial value. */
	poke32(TCU_OSTCNTH, 0);
	poke32(TCU_OSTCNTL, 0);

	/* Use EXTCLK as the clock source */
	poke32(TCU_OSTCSR, peek32(TCU_OSTCSR) | OSTCSR_EXT_EN);

	/* Comparison value -- once every MS. */
	poke32(TCU_OSTDR, OS_TIMER_HZ / 1000);

	/* Enable the timer*/
	poke32(TESR, TER_OSTEN);

	/* Register for interrupts */
	intc_register_handler_tcu0(ostimer_interrupt);

	/* Unmask timer IRQ, in TCU */
	poke32(TMCR, TMR_OSTMASK);

	/* Unmask timer IRQ, in interrupt controller */
	intc_irq_unmask_tcu0();
}

void ostimer_interrupt(void)
{
	for(int i = 0; i < timer_callback_count; i++)
		timer_callbacks[i]();

	/* Clear interrupt flag. If we don't do this we will immediately return to
	 * this interrupt on exit! */
	poke32(TFCR, TFR_OSTFLAG);
}

int ostimer_register_callback(timer_callback callback)
{
	if(timer_callback_count == MAX_TIMER_CALLBACKS)
		return 0;

	timer_callbacks[timer_callback_count] = callback;

	timer_callback_count++;
	return 1;
}

