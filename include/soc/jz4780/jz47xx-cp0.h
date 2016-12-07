#ifndef ASM
#include "architecture/cpufunc.h"
#endif

#define CP0_CORE_CTL $12, 2
/* JZ47xx-specific CP0 registers */

#define CP0_CORE_CTL_SLEEP1M (1 << 17)
#define CP0_CORE_CTL_SLEEP0M (1 << 16)
#define CP0_CORE_CTL_RPC1 (1 << 9)
#define CP0_CORE_CTL_RPC0 (1 << 8)
#define CP0_CORE_CTL_SW_RST1 (1 << 1)
#define CP0_CORE_CTL_SW_RST0 (1 << 0)

#define CP0_CORE_STATUS $12, 3
#define CP0_CORE_STATUS_MIRQ1_P (1 << 1)
#define CP0_CORE_STATUS_MIRQ0_P (1 << 0)

#define CP0_CORE_REIM $12, 4
#define CP0_CORE_REIM_ENTRY_MASK 0xFFFF0000
#define CP0_CORE_REIM_MIRQ1_M (1 << 1)
#define CP0_CORE_REIM_MIRQ0_M (1 << 0)

#define CP0_CORE_MBOX0 $20, 0
#define CP0_CORE_MBOX1 $20, 1

#ifndef ASM
MIPS_CP0_OPS(xburst_core_ctl,12,2)
MIPS_CP0_OPS(xburst_core_status,12,3)
MIPS_CP0_OPS(xburst_core_reim,12,4)
#endif

/* jz47xx hardware interrupts */
// #define CP0_HARD_IRQ_IPI (1 + CP0_HARD_IRQ_BASE)


