#include "debug_uart.h"
#include "peekpoke.h"
#include "ci20board.h"

void debug_uart_init(void)
{
	unsigned int divisor = EXTCLK / 16 / 115200;

	write_l(UART4_ULCR, UART_ULCR_WLS_8); 
	write_l(UART4_UMCR, UART_UMCR_RTS); 
	write_l(UART4_UFCR, UART_UFCR_RTDR_16 | UART_UFCR_UME | UART_UFCR_TFRT | UART_UFCR_RFRT | UART_UFCR_FME);
	write_l(UART4_UDLLR, divisor & 0xff);
	write_l(UART4_UDLHR, (divisor & 0xff00) >> 8);
}

void debug_uart_putc(char c)
{
	while((read_l(UART4_ULSR) & 0x20) == 0) /* Transmit-hold register empty */
		;

	write_l(UART4_UTHR, c);
}

void debug_uart_puts(char *s)
{
	for(;*s;s++)
		debug_uart_putc(*s);
}

