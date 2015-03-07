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
#define UART_UMCR_RTS  (1 << 1) /* RTS */

#define UART_UFCR_RTDR_1  (0 << 6)
#define UART_UFCR_RTDR_16 (1 << 6)
#define UART_UFCR_RTDR_32 (2 << 6)
#define UART_UFCR_RTDR_60 (3 << 6)
#define UART_UFCR_UME     (1 << 4) /* Enable the UART */
#define UART_UFCR_DMR     (1 << 3) /* DMA enable */
#define UART_UFCR_TFRT    (1 << 2) /* Transmit holding register reset */
#define UART_UFCR_RFRT    (1 << 1) /* Receive buffer reset */
#define UART_UFCR_FME     (1 << 0) /* FIFO mode enable */

#define UART_ULSR_FIFOE (1 << 7) /* FIFO Error Status. (FIFO mode only) */
#define UART_ULSR_TEMP  (1 << 6) /* Transmit Holding Register Empty. */
#define UART_ULSR_TDRQ  (1 << 5) /* Transmit Data Request. */
#define UART_ULSR_BI    (1 << 4) /* Break Interrupt. */
#define UART_ULSR_FMER  (1 << 3) /* Framing Error. */
#define UART_ULSR_PARER (1 << 2) /* Parity Error. */
#define UART_ULSR_OVER  (1 << 1) /* Overrun Error. */
#define UART_ULSR_DRY   (1 << 0) /* Data Ready. */

void debug_uart_init(void);
void debug_uart_putc(char c);
void debug_uart_puts(char *s);

