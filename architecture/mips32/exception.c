#include <inttypes.h>
#include <architecture/cpufunc.h>
#include <architecture/cp0.h>
#include <kernel/driver/jz47xx-uart/jz47xx-uart.h>
#include <architecture/userstate.h>

extern struct architecture_userstate current;

static void mips_syscall(uint32_t arg0, uint32_t arg1, uint32_t arg2)
{
	switch(current.v0) {
		case 1: // putc 
			uart_putc(arg0);
			break;
		default:
			uart_print("syscall %x4 %x4 %x4 %x4\r\n", current.v0, arg0, arg1, arg2);
	}

	current.cp0_EPC += 4;
}

void mips_exception(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t cause)
{
	/* Saved user state in current */
	int exccode = (cause & CP0_CAUSE_EXCCODE_MASK) >> CP0_CAUSE_EXCCODE_SHIFT;

	switch(exccode) {
		case CP0_CAUSE_EXCCODE_Sys:
			mips_syscall(arg0, arg1, arg2);
			break;
		default:
			/* Can't handle it -- panic for now (later: kill current?) */
			uart_print("unknown exc exccode %x4 cause %x4\r\n", exccode, cause);
			uart_print("badvaddr %x4\r\n", mips_read_cp0_badvaddr());
			while(1) {
				__asm__("wait");
			}
			break;
	}
	
}

