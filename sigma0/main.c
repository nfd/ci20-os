/* Our stack -- we get one per hardware thread */
#include <inttypes.h>
#include <soc/config.h>
#include <architecture/memory.h>
#include <syscalls.h>

#define STACK_SIZE 4096

uint8_t stack[NUM_CORES][STACK_SIZE] __attribute__ ((aligned (PAGE_SIZE))) ;

void entrypoint(int core_num)
{
	syscall_putchar_debug('s');
	syscall_putchar_debug('u');
	syscall_putchar_debug('p');
	syscall_putchar_debug('0' + core_num);
	syscall_putchar_debug('\r');
	syscall_putchar_debug('\n');

	/* Attempt to access kernel space -- should cause an exception */
	/*
	volatile uint8_t *kernel = (uint8_t *)0x80000000;
	*kernel++;
	*/

	while(1) {
		__asm__("wait");
	}
}

