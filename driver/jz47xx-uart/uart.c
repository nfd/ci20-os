#include <stdarg.h>

#include "driver/jz47xx-uart/jz47xx-uart.h"
#include "architecture/peekpoke.h"
#include "ci20board.h"
#include "inttypes.h"

#define UART_BASE 0xb0030000

/* Uarts 0 to 4 are defined */
#define UART(num) (UART_BASE + (num * 0x1000))

/* Uart registers */
#define URBR 0x0
#define UTHR 0x0
#define UDLLR 0x0
#define UDLHR 0x4
#define UIER 0x4
#define UIIR 0x8
#define UFCR 0x8
#define ULCR 0x0c
#define UMCR 0x10
#define ULSR 0x14
#define UMSR 0x18
#define USPR 0x1c
#define ISR 0x20
#define UMR 0x24
#define UACR 0x28
#define URCR 0x40
#define UTCR 0x44

/* Fields within registers */
#define ULCR_DLAB  (1 << 7) /* Divisor latch access */
#define ULCR_SBK   (1 << 6) /* Set break */
#define ULCR_STPAR (1 << 5) /* Stick parity*/
#define ULCR_PARM  (1 << 4) /* 1 = even parity, 0 = odd parity */
#define ULCR_PARE  (1 << 3) /* Parity enable */
#define ULCR_SBLS  (1 << 2) /* 0 = 1 stop bit, 1 = 2 stop bits */
#define ULCR_WLS_5 (0) /* 5-bit words */
#define ULCR_WLS_6 (1) /* 6-bit words */
#define ULCR_WLS_7 (2) /* 7-bit words */
#define ULCR_WLS_8 (3) /* 8-bit words */

#define UMCR_MDCE (1 << 7) /* Modem control enable */
#define UMCR_FCM  (1 << 6) /* 1 = hardware, 0 = software */
#define UMCR_LOOP (1 << 4) /* Loopback testing mode */
#define UMCR_RTS  (1 << 1) /* RTS */

#define UFCR_RTDR_1  (0 << 6)
#define UFCR_RTDR_16 (1 << 6)
#define UFCR_RTDR_32 (2 << 6)
#define UFCR_RTDR_60 (3 << 6)
#define UFCR_UME     (1 << 4) /* Enable the UART */
#define UFCR_DMR     (1 << 3) /* DMA enable */
#define UFCR_TFRT    (1 << 2) /* Transmit holding register reset */
#define UFCR_RFRT    (1 << 1) /* Receive buffer reset */
#define UFCR_FME     (1 << 0) /* FIFO mode enable */

#define ULSR_FIFOE (1 << 7) /* FIFO Error Status. (FIFO mode only) */
#define ULSR_TEMP  (1 << 6) /* Transmit Holding Register Empty. */
#define ULSR_TDRQ  (1 << 5) /* Transmit Data Request. */
#define ULSR_BI    (1 << 4) /* Break Interrupt. */
#define ULSR_FMER  (1 << 3) /* Framing Error. */
#define ULSR_PARER (1 << 2) /* Parity Error. */
#define ULSR_OVER  (1 << 1) /* Overrun Error. */
#define ULSR_DRY   (1 << 0) /* Data Ready. */


#define UART_BPS 115200

static const int uart_num = 4;

static inline void uart_w(uint32_t addr, uint32_t val) {
	poke32(UART(uart_num) + addr, val);
}

static inline uint32_t uart_r(uint32_t addr) {
	return peek32(UART(uart_num) + addr);
}

static void board_specific_init(void)
{
	/* Enable UART 4 GPIOs */
	poke32(GPIO(C, INTC),  PIN_UART4_TXD | PIN_UART4_RXD); // GPIO/Device, not IRQ
	poke32(GPIO(C, MSKC),  PIN_UART4_TXD | PIN_UART4_RXD); // Device, not GPIO
	poke32(GPIO(C, PAT1S), PIN_UART4_TXD | PIN_UART4_RXD); // Device 2 or 3
	poke32(GPIO(C, PAT0C), PIN_UART4_TXD | PIN_UART4_RXD); // Device 2

	/* Enable UART4 clock. UARTs are clocked from EXTCLK: no PLL required. */
	poke32(CPM_CLKGR1, peek32(CPM_CLKGR1) & ~CLKGR1_UART4);
}

void uart_init(void)
{
	/* The jz47xx UART is like a 16550, except that it also needs a "uart module enable"
	 * bit to be set (UART_UFCR_UME) before it will do anything.
	*/
	const uint32_t uart_divisor = EXTCLK / 16 / UART_BPS;

	board_specific_init();

	/* Disable UART4 interrupts */
	uart_w(UIER, 0);

	/* Modem control: RTS */
	uart_w(UMCR, UMCR_RTS); 

	/* Enable FIFO and reset rx and tx */
	uart_w(UFCR, UFCR_FME | UFCR_TFRT | UFCR_RFRT);

	/* Enable banking and set the baud rate */
	uart_w(ULCR, ULCR_DLAB | ULCR_WLS_8); 
	uart_w(UDLLR, uart_divisor & 0xff);
	uart_w(UDLHR, (uart_divisor & 0xff00) >> 8);

	/* Disable banking again */
	uart_w(ULCR, ULCR_WLS_8); 

	/* Enable the module (jz4780-specific step) */
	uart_w(UFCR, UFCR_FME | UFCR_TFRT | UFCR_RFRT | UFCR_UME);
}

void uart_putc(char c)
{
	while((uart_r(ULSR) & ULSR_TDRQ) == 0) /* Transmit-hold register empty */
		;

	uart_w(UTHR, c);
}

char uart_getc(void)
{
	while((uart_r(ULSR) & ULSR_DRY) == 0) /* Data-ready register not set */
		;

	return (char)(uart_r(URBR) & 0xFF);
}

void uart_puts(char *s)
{
	for(;*s;s++)
		uart_putc(*s);
}

static void _puthex(int val)
{
	if(val <= 9) {
		uart_putc('0' + val);
	} else {
		uart_putc('A' + val - 10);
	}
}

void uart_puthex32(uint32_t val)
{
	for(int i = 0; i < 8; i++) {
		_puthex((val & 0xF0000000) >> 28);
		val <<= 4;
	}
}

void uart_puthex64(uint64_t val)
{
	uart_puthex32(val >> 32);
	uart_puthex32(val & 0xFFFFFFFF);
}

/* Super-mini printf. Recognises:
 *
 * All backslash sequences (C compiler does this for us)
 * %x4 - 32-bit hex value (nb both characters required)
 * %x8 - 64-bit hex value (as above)
 * %c  - a char
 * %s  - a c string
*/
void uart_print(char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	for(;*fmt;) {
		char c = *fmt++;
		if(c == '%') {
			switch(*fmt++) {
			case 'x':
				switch(*fmt++) {
				case '4':
					uart_puthex32(va_arg(args, uint32_t));
					break;
				case '8':
					uart_puthex64(va_arg(args, uint64_t));
					break;
				}
				break;
			case 'c':
				uart_putc(va_arg(args, int));
				break;
			case 's':
				uart_puts(va_arg(args, char *));
				break;
			}
		} else {
			uart_putc(c);
		}
	}
	va_end(args);
}

