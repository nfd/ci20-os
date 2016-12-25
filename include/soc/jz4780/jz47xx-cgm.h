#include "architecture/peekpoke.h"

/* Clock gating */
#define XBURST_CGM_BASE 0xB0000000

#define XBURST_CGM_LPCR    (XBURST_CGM_BASE + 0x4)
#define XBURST_CGM_CLKGR0  (XBURST_CGM_BASE + 0x20)
#define XBURST_CGM_CLKGR1  (XBURST_CGM_BASE + 0x28)

#define XBURST_CGM_GATE_UNGATE(name, reg, val) \
	static inline void xburst_cgm_ungate_ ## name() { \
		MIPS_CLEAR_BIT(reg, val); \
	} \
	static inline void xburst_cgm_gate_ ## name() { \
		MIPS_SET_BIT(reg, val); \
	}

XBURST_CGM_GATE_UNGATE(p1, XBURST_CGM_CLKGR1, 15)
XBURST_CGM_GATE_UNGATE(x2d, XBURST_CGM_CLKGR1, 14)
XBURST_CGM_GATE_UNGATE(des, XBURST_CGM_CLKGR1, 13)
XBURST_CGM_GATE_UNGATE(smb4, XBURST_CGM_CLKGR1, 12)
XBURST_CGM_GATE_UNGATE(ahb_mon, XBURST_CGM_CLKGR1, 11)
XBURST_CGM_GATE_UNGATE(uart4, XBURST_CGM_CLKGR1, 10)
XBURST_CGM_GATE_UNGATE(hdmi, XBURST_CGM_CLKGR1, 9)
XBURST_CGM_GATE_UNGATE(otg1, XBURST_CGM_CLKGR1, 8)
XBURST_CGM_GATE_UNGATE(gpvlc, XBURST_CGM_CLKGR1, 7)
XBURST_CGM_GATE_UNGATE(aic1, XBURST_CGM_CLKGR1, 6)
XBURST_CGM_GATE_UNGATE(compress, XBURST_CGM_CLKGR1, 5)
XBURST_CGM_GATE_UNGATE(gpu, XBURST_CGM_CLKGR1, 4)
XBURST_CGM_GATE_UNGATE(pcm, XBURST_CGM_CLKGR1, 3)
XBURST_CGM_GATE_UNGATE(vpu, XBURST_CGM_CLKGR1, 2)
XBURST_CGM_GATE_UNGATE(tssi1, XBURST_CGM_CLKGR1, 1)
XBURST_CGM_GATE_UNGATE(smb3, XBURST_CGM_CLKGR1, 0)

#define XBURST_CGM_LPCR_CONTROL(name, bit) \
	static inline void xburst_cgm_lpcr_ ## name ## _up() { \
		MIPS_CLEAR_BIT(XBURST_CGM_LPCR, bit); \
	}; \
	static inline void xburst_cgm_lpcr_ ## name ## _down() { \
		MIPS_SET_BIT(XBURST_CGM_LPCR, bit); \
	}; \
	static inline unsigned int xburst_cgm_lpcr_ ## name ## _is_down() { \
		return (peek32(XBURST_CGM_LPCR) & (1 << bit)); \
	};

XBURST_CGM_LPCR_CONTROL(pd_scpu, 31)

