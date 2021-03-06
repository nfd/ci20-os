#include <kernel/driver/jz47xx-uart/jz47xx-uart.h>
#include <architecture/kernel_layout.h>
#include <kernel/soc.h>

#include "sigma0_interface.h"

extern struct architecture_memory_layout kernel_memory_layout;

extern void jump_to_sigma0_asm(int core_num, void *address);

void jump_to_sigma0()
{
	uart_puts("jump to sigma0 at ");
	uart_puthex32((uint32_t)(kernel_memory_layout.sigma0_entrypoint));
	uart_puts("\r\n");

	// enter user-space with eret.
	jump_to_sigma0_asm(soc_get_core_number(), kernel_memory_layout.sigma0_entrypoint);

	uart_puts("Did it happen?\r\n");

	// We never get here.
	while(1)
		__asm__("wait");
}

