#include "debug_uart.h"
#include "peekpoke.h"
#include "ci20board.h"

#define DEBUG_UART_BPS 115200

#define DEBUG_UART_DIVISOR (EXTCLK / 16 / DEBUG_UART_BPS)

void debug_uart_init(void)
{
	/* The jz47xx UART is like a 16550, except that it also needs a "uart module enable"
	 * bit to be set (UART_UFCR_UME) before it will do anything.
	 *
	 * Note that board init must be completed first to enable the UART-specific
	 * GPIOs and the UART clock. 
	*/

	/* Disable UART4 interrupts */
	poke32(UART4_UIER, 0);

	/* Modem control: RTS */
	poke32(UART4_UMCR, UART_UMCR_RTS); 

	/* Enable FIFO and reset rx and tx, and enable the module (this last is
	 * jz47xx-specific). */
	poke32(UART4_UFCR, UART_UFCR_FME | UART_UFCR_TFRT | UART_UFCR_RFRT | UART_UFCR_UME);

	/* Enable banking and set the baud rate */
	poke32(UART4_ULCR, UART_ULCR_DLAB | UART_ULCR_WLS_8); 
	poke32(UART4_UDLLR, DEBUG_UART_DIVISOR & 0xff);
	poke32(UART4_UDLHR, (DEBUG_UART_DIVISOR & 0xff00) >> 8);

	/* Disable banking again */
	poke32(UART4_ULCR, UART_ULCR_WLS_8); 
}

void debug_uart_putc(char c)
{
	while((peek32(UART4_ULSR) & UART_ULSR_TDRQ) == 0) /* Transmit-hold register empty */
		;

	poke32(UART4_UTHR, c);
}

void debug_uart_puts(char *s)
{
	for(;*s;s++)
		debug_uart_putc(*s);
}

