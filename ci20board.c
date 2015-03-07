#include "inttypes.h"
#include "peekpoke.h"
#include "ci20board.h"

void ci20board_init(void)
{
	/* Enable UART 4 GPIOs */
	poke32(GPIO(C, INTC),  PIN_UART4_TXD | PIN_UART4_RXD); // GPIO/Device, not IRQ
	poke32(GPIO(C, MSKC),  PIN_UART4_TXD | PIN_UART4_RXD); // Device, not GPIO
	poke32(GPIO(C, PAT1S), PIN_UART4_TXD | PIN_UART4_RXD); // Device 2 or 3
	poke32(GPIO(C, PAT0C), PIN_UART4_TXD | PIN_UART4_RXD); // Device 2

	/* Enable UART4 clock. UARTs are clocked from EXTCLK: no PLL required. */
	poke32(CPM_CLKGR1, peek32(CPM_CLKGR1) & ~CPM_CLKGR1_UART4);
}

