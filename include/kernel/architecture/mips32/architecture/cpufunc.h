#ifndef MIPS_CPUFUNC_H
#define MIPS_CPUFUNC_H

#include <inttypes.h>
#include <stringify.h>
#include <architecture/cp0.h>

#define MIPS_KSEG0_TO_KSEG1(addr) (addr | 0x20000000)

#define MIPS_CP0_OPS(name,num,sel) \
	static inline uint32_t mips_read_cp0_ ## name (void) { \
		int result; \
		__asm("mfc0 %[v0], $" XSTRING(num) "," XSTRING(sel) ";" \
				"ehb;" \
				: [v0] "=&r"(result)); \
		return result; \
	} \
	static inline void mips_write_cp0_ ## name (uint32_t val) { \
		__asm("mtc0 %[a0], $" XSTRING(num) "," XSTRING(sel) ";" \
				"ehb;" \
				: : [a0] "r"(val)); \
	}

#define MIPS_CLEAR_BIT(reg, bit) poke32(reg, peek32(reg) & ~(1 << (bit)))
#define MIPS_SET_BIT(reg, bit) poke32(reg, peek32(reg) | (1 << (bit)))

MIPS_CP0_OPS(badvaddr, CP0_BADVADDR, 0)

#endif // MIPS_CPUFUNC_H

