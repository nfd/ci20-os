/* Our stack -- we get one per hardware thread */
#include <inttypes.h>
#include <soc/config.h>
#include <architecture/memory.h>
#include <syscalls.h>

#define STACK_SIZE 4096

uint8_t stack[NUM_CORES][STACK_SIZE] __attribute__ ((aligned (PAGE_SIZE))) ;

volatile uint8_t word = 0;

static void increment(void)
{
	while(1)
		word ++;
}

static inline char to_hex(uint8_t num)
{
	return num < 10 ? '0' + num : ('A' - 10) + num;
}

static void print(void)
{
	while(1) {
		uint8_t current = word;
		syscall_putchar_debug(to_hex(current >> 4));
		syscall_putchar_debug(to_hex(current & 0xf));

		for(volatile uint32_t delay = 0; delay < 0xfffffff; delay++)
			;
	}
}

void entrypoint(int core_num)
{
	syscall_putchar_debug('s');
	syscall_putchar_debug('u');
	syscall_putchar_debug('p');
	syscall_putchar_debug('0' + core_num);
	syscall_putchar_debug('\r');
	syscall_putchar_debug('\n');

	if(core_num == 0) {
		increment();
	} else {
		print();
	}

	while(1) {
		__asm__("wait");
	}
}

