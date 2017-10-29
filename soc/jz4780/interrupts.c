#include <inttypes.h>
#include <kernel/soc/jz4780/jz47xx-interrupts.h>
#include <kernel/driver/jz47xx-uart/jz47xx-uart.h>

/* Interrupt dispatch table */
#define IRQ_COUNT 64

static irqhandler handlers[IRQ_COUNT];

void intc_register_handler_raw(int index, irqhandler handler)
{
	handlers[index] = handler;
}

static void intc_dummy_handler(void)
{
	uart_puts("Ignoring unexpected interrupt!\r\n");
}

void intc_init(void)
{
	for(int i = 0; i < IRQ_COUNT; i++)
		intc_register_handler_raw(i, intc_dummy_handler);
}

/* Helper function for main interrupt dispatch routine */
static inline void libci20_interrupt_dispatch_bank(int offset)
{
	uint32_t pending = peek32(INTC_ICPR0 + offset);
	int idx;

	while((idx = __builtin_ffs(pending))) {
		idx --;

		handlers[idx + offset]();

		pending &= ~(1 << idx);
	}
}

/* Main interrupt dispatch routine, called from asm (see kernel/start.S) */
void libci20_interrupt(void)
{
	libci20_interrupt_dispatch_bank(0);
	libci20_interrupt_dispatch_bank(0x20);
}

