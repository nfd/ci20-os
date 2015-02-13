#define GPIO_F_SET 0xb0010544
#define GPIO_F_CLEAR 0xb0010548

#define UART4_BASE 0xb0034000
#define UART4_URBR (UART4_BASE + 0x0)
#define UART4_UTHR (UART4_BASE + 0x0)
#define UART4_UDLLR (UART4_BASE + 0x0)
#define UART4_UDLHR (UART4_BASE + 0x4)
#define UART4_UIER (UART4_BASE + 0x4)
#define UART4_UIIR (UART4_BASE + 0x8)
#define UART4_UFCR (UART4_BASE + 0x8)
#define UART4_ULCR (UART4_BASE + 0x0c)
#define UART4_UMCR (UART4_BASE + 0x10)
#define UART4_ULSR (UART4_BASE + 0x14)
#define UART4_UMSR (UART4_BASE + 0x18)
#define UART4_USPR (UART4_BASE + 0x1c)
#define UART4_ISR (UART4_BASE + 0x20)
#define UART4_UMR (UART4_BASE + 0x24)
#define UART4_UACR (UART4_BASE + 0x28)
#define UART4_URCR (UART4_BASE + 0x40)
#define UART4_UTCR (UART4_BASE + 0x44)

#define UART_ULCR_DLAB  (1 << 7) /* Divisor latch access */
#define UART_ULCR_SBK   (1 << 6) /* Set break */
#define UART_ULCR_STPAR (1 << 5) /* Stick parity*/
#define UART_ULCR_PARM  (1 << 4) /* 1 = even parity, 0 = odd parity */
#define UART_ULCR_PARE  (1 << 3) /* Parity enable */
#define UART_ULCR_SBLS  (1 << 2) /* 0 = 1 stop bit, 1 = 2 stop bits */
#define UART_ULCR_WLS_5 (0) /* 5-bit words */
#define UART_ULCR_WLS_6 (1) /* 6-bit words */
#define UART_ULCR_WLS_7 (2) /* 7-bit words */
#define UART_ULCR_WLS_8 (3) /* 8-bit words */

#define UART_UMCR_MDCE (1 << 7) /* Modem control enable */
#define UART_UMCR_FCM  (1 << 6) /* 1 = hardware, 0 = software */
#define UART_UMCR_LOOP (1 << 4) /* Loopback testing mode */
#define UART_UMCR_RTS  (1 << 1) /* Force RTS high */

#define UART_UFCR_RTDR_1  (0 << 6)
#define UART_UFCR_RTDR_16 (1 << 6)
#define UART_UFCR_RTDR_32 (2 << 6)
#define UART_UFCR_RTDR_60 (3 << 6)
#define UART_UFCR_UME     (1 << 4) /* Enable the UART */
#define UART_UFCR_DMR     (1 << 3) /* DMA enable */
#define UART_UFCR_TFRT    (1 << 2) /* Transmit holding register reset */
#define UART_UFCR_RFRT    (1 << 1) /* Receive buffer reset */
#define UART_UFCR_FME     (1 << 0) /* FIFO mode enable */

#define GPIO_F_LED_PIN (1 << 15)

#define EXTCLK 48000000 /* 48 MHz external clock driving the serial line */

static inline void write_l(unsigned int addr, unsigned int val)
{
	volatile unsigned int *ptr = (unsigned int *)(addr);

	*ptr = val;
}

static inline unsigned int read_l(unsigned int addr)
{
	volatile unsigned int *ptr = (unsigned int *)(addr);

	return *ptr;
}

static void uart_init(void)
{
	unsigned int divisor = EXTCLK / 16 / 115200;

	write_l(UART4_ULCR, UART_ULCR_WLS_8); 
	write_l(UART4_UMCR, UART_UMCR_RTS); 
	write_l(UART4_UFCR, UART_UFCR_RTDR_16 | UART_UFCR_UME | UART_UFCR_TFRT | UART_UFCR_RFRT | UART_UFCR_FME);
	write_l(UART4_UDLLR, divisor & 0xff);
	write_l(UART4_UDLHR, (divisor & 0xff00) >> 8);
}

static void uart_putc(char c)
{
	while((read_l(UART4_ULSR) & 0x20) == 0) /* Transmit-hold register empty */
		;

	write_l(UART4_UTHR, c);
}

static void uart_puts(char *s)
{
	for(;*s;s++)
		uart_putc(*s);
}

void entrypoint(void)
{
	uart_init();

	/* LED blue to say we've arrived */
	write_l(GPIO_F_CLEAR, GPIO_F_LED_PIN); /* Turn LED blue */

	uart_puts("Hello, world!\n");

	while(1)
		;
}

