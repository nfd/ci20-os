#include "architecture/peekpoke.h"
#include "ci20board.h"

typedef void(*irqhandler)(void);

/*
 * This header file contains preprocessor magic to create these functions, for every IRQ:
 *
 * int intc_irq_is_set_NAME(void);
 * void intc_irq_mask_NAME(void);
 * void intc_irq_unmask_NAME(void);
 * void intc_register_handler_NAME(irqhandler);
 *
 * where NAME is one of the names supplied to _intc_make_irq_funcs below, such as tcu0.
 *
*/

void intc_register_handler_raw(int index, irqhandler handler);
void intc_init(void);

#define BANK(bank) (bank * 0x20)

#define _intc_irq_is_set(name, bank, bit) \
inline int intc_irq_is_set_ ## name(void) { \
	return (peek32(INTC_ICSR0 + BANK(bank)) & (1 << bit));\
}

#define _intc_irq_mask(name, bank, bit) \
inline void intc_irq_mask_ ## name(void) { \
	poke32(INTC_ICMSR0 + BANK(bank), (1 << bit)); \
}

#define _intc_irq_unmask(name, bank, bit) \
inline void intc_irq_unmask_ ## name(void) { \
	poke32(INTC_ICMCR0 + BANK(bank), (1 << bit)); \
}

#define _intc_register_handler(name, bank, bit) \
inline void intc_register_handler_ ## name(irqhandler handler) { \
	intc_register_handler_raw(bit + (bank * 0x20), handler); \
}

#define _intc_make_irq_funcs(name, bank, bit) \
	_intc_irq_is_set(name, bank, bit) \
	_intc_irq_mask(name, bank, bit) \
	_intc_irq_unmask(name, bank, bit) \
	_intc_register_handler(name, bank, bit)

/* Interrupt sources, register 0 */
_intc_make_irq_funcs(lcd, 0, 31)
_intc_make_irq_funcs(cim, 0, 30)
_intc_make_irq_funcs(ipu, 0, 29)
_intc_make_irq_funcs(gps, 0, 28)
_intc_make_irq_funcs(tcu0, 0, 27)
_intc_make_irq_funcs(tcu1, 0, 26)
_intc_make_irq_funcs(tcu2, 0, 25)
_intc_make_irq_funcs(gps_1ms, 0, 24)
_intc_make_irq_funcs(lcd1, 0, 23)
_intc_make_irq_funcs(ipu1, 0, 22)
_intc_make_irq_funcs(otg, 0, 21)
_intc_make_irq_funcs(ehci, 0, 20)
_intc_make_irq_funcs(x2d, 0, 19)
_intc_make_irq_funcs(sadc, 0, 18)
_intc_make_irq_funcs(gpio0, 0, 17)
_intc_make_irq_funcs(gpio1, 0, 16)
_intc_make_irq_funcs(gpio2, 0, 15)
_intc_make_irq_funcs(gpio3, 0, 14)
_intc_make_irq_funcs(gpio4, 0, 13)
_intc_make_irq_funcs(gpio5, 0, 12)
_intc_make_irq_funcs(tssi1, 0, 11)
_intc_make_irq_funcs(pdma, 0, 10)
_intc_make_irq_funcs(tssi0, 0, 9)
_intc_make_irq_funcs(ssi0, 0, 8)
_intc_make_irq_funcs(ssi1, 0, 7)
_intc_make_irq_funcs(reserved_0_6, 0, 6)
_intc_make_irq_funcs(ohci, 0, 5)
_intc_make_irq_funcs(hdmi_wakeup, 0, 4)
_intc_make_irq_funcs(hdmi, 0, 3)
_intc_make_irq_funcs(bch, 0, 2)
_intc_make_irq_funcs(aic0, 0, 1)
_intc_make_irq_funcs(aic1, 0, 0)

/* Interrupt sources, set 1 */
_intc_make_irq_funcs(gpu, 1, 31)
_intc_make_irq_funcs(vpu, 1, 30)
_intc_make_irq_funcs(pdmam, 1, 29)
_intc_make_irq_funcs(smb0, 1, 28)
_intc_make_irq_funcs(smb1, 1, 27)
_intc_make_irq_funcs(smb2, 1, 26)
_intc_make_irq_funcs(smb3, 1, 25)
_intc_make_irq_funcs(smb4, 1, 24)
_intc_make_irq_funcs(ethc, 1, 23)
_intc_make_irq_funcs(nemc, 1, 22)
_intc_make_irq_funcs(reserved_1_21, 1, 21)
_intc_make_irq_funcs(ddr, 1, 20)
_intc_make_irq_funcs(uart0, 1, 19)
_intc_make_irq_funcs(uart1, 1, 18)
_intc_make_irq_funcs(uart2, 1, 17)
_intc_make_irq_funcs(uart3, 1, 16)
_intc_make_irq_funcs(cpm, 1, 15)
_intc_make_irq_funcs(harb0, 1, 14)
_intc_make_irq_funcs(reserved_1_13, 1, 13)
_intc_make_irq_funcs(harb2, 1, 12)
_intc_make_irq_funcs(cOMPRESS, 1, 11)
_intc_make_irq_funcs(gpvlc, 1, 10)
_intc_make_irq_funcs(kbc, 1, 9)
_intc_make_irq_funcs(pcm0, 1, 8)
_intc_make_irq_funcs(reserved_1_7, 1, 7)
_intc_make_irq_funcs(scc, 1, 6)
_intc_make_irq_funcs(msc0, 1, 5)
_intc_make_irq_funcs(msc1, 1, 4)
_intc_make_irq_funcs(msc2, 1, 3)
_intc_make_irq_funcs(uart4, 1, 2)
_intc_make_irq_funcs(owi, 1, 1)
_intc_make_irq_funcs(rtc, 1, 0)

#undef _intc_make_irq_funcs
#undef _intc_register_handler
#undef _intc_irq_unmask
#undef _intc_irq_mask
#undef _intc_irq_is_set
#undef BANK

