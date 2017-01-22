#include <inttypes.h>
#include <kernel/soc/jz4780/jz47xx-devices.h>

void uart_init(void);
void uart_putc(char c);
void uart_puts(char *s);
char uart_getc(void);
void uart_puthex8(unsigned char val);
void uart_puthex32(unsigned int val);
void uart_puthex64(uint64_t val);

void uart_print(char *fmt, ...);

