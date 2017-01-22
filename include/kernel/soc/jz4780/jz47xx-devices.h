/* Devices */
#define CPM_BASE   0xb0000000
#define INTC_BASE  0xb0001000
#define GPIO_BASE  0xb0010000
#define DDR_BASE   0xb3010000
#define DDRP_BASE  0xb3011000
#define MSC0_BASE  0xb3450000
#define MSC1_BASE  0xb3460000
#define MSC2_BASE  0xb3470000

/* Clock Reset and Power Controller */
#define CPM_CPCCR  (CPM_BASE + 0x0)
#define CPM_CPPCR  (CPM_BASE + 0xC)
#define CPM_CPCSR  (CPM_BASE + 0xD4)
#define CPM_CPAPCR (CPM_BASE + 0x10)
#define CPM_CPMPCR (CPM_BASE + 0x14)
#define CPM_CLKGR0 (CPM_BASE + 0x20)
#define CPM_CLKGR1 (CPM_BASE + 0x28)
#define CPM_DDRCDR (CPM_BASE + 0x2C)
#define CPM_DRCG   (CPM_BASE + 0xD0)

/* Affects CPCCR */
#define CPCCR_SEL_SRC_APLL     (1 << 30)
#define CPCCR_SEL_SRC_EXTCLK   (2 << 30)
#define CPCCR_SEL_SRC_RTCLK    (3 << 30)
#define CPCCR_SEL_CPLL_SCLK_A  (1 << 28)
#define CPCCR_SEL_CPLL_MPLL    (2 << 28)
#define CPCCR_SEL_CPLL_EPLL    (3 << 28)
#define CPCCR_SEL_H0PLL_SCLK_A (1 << 26)
#define CPCCR_SEL_H0PLL_MPLL   (2 << 26)
#define CPCCR_SEL_H0PLL_EPLL   (3 << 26)
#define CPCCR_SEL_H2PLL_SCLK_A (1 << 24)
#define CPCCR_SEL_H2PLL_MPLL   (2 << 24)
#define CPCCR_SEL_H2PLL_TCK    (3 << 24)
#define CPCCR_CE_CPU           (1 << 22)
#define CPCCR_CE_AHB0          (1 << 21)
#define CPCCR_CE_AHB2          (1 << 20)
#define CPCCR_PDIV_SHIFT       16
#define CPCCR_H2DIV_SHIFT      12
#define CPCCR_H0DIV_SHIFT      8
#define CPCCR_L2CDIV_SHIFT     4
#define CPCCR_CDIV_SHIFT       0
#define CPCCR_CLOCK_SRC_MASK   0xFF000000

/* Affects CPPCR */
#define CPPCR_BWADJ_MASK       0x000FFF00
#define CPPCR_BWADJ_SHIFT      8

/* Affects CPCSR */
#define CPCSR_H2DIV_BUSY (1 << 2)
#define CPCSR_H0DIV_BUSY (1 << 1)
#define CPCSR_CDIV_BUSY (1 << 0)

/* Affects CPAPCR, CPMPCR */
#define PLL_M_SHIFT    19
#define PLL_N_SHIFT    13
#define PLL_OD_SHIFT    9
#define PLL_MNOD_MASK  0xFFFFFF00
#define PLL_ON_BIT     (1 << 4)
#define PLL_ENABLE_BIT (1 << 0)

/* Affects CLKGR0 */
#define CLKGR0_DDR1   (1 << 31)
#define CLKGR0_DDR0   (1 << 30)

/* Affects CLKGR1 */
#define CLKGR1_UART4  (1 << 10)

/* INTC - interrupt controller */
#define INTC_ICSR0    (INTC_BASE + 0x0)
#define INTC_ICMSR0   (INTC_BASE + 0x8)
#define INTC_ICMCR0   (INTC_BASE + 0xC)
#define INTC_ICPR0    (INTC_BASE + 0x10)
#define INTC_ICSR1    (INTC_BASE + 0x20)
#define INTC_ICPR1    (INTC_BASE + 0x30)

#define INTC_ICMR0_TCU0 (1 << 27)
#define INTC_ICMR0_TCU1 (1 << 26)
#define INTC_ICMR0_TCU2 (1 << 25)

/* Affects DDRCDR */
#define DDRCDR_DCS_STOP    (0 << 30)
#define DDRCDR_DCS_SCLK_A  (1 << 30)
#define DDRCDR_DCS_MPLL    (2 << 30)
#define DDRCDR_CE_DDR      (1 << 29)
#define DDRCDR_DDR_BUSY    (1 << 28)
#define DDRCDR_DDR_STOP    (1 << 27)
#define DDRCDR_CDR_SHIFT   0

/* DDR controller */
#define DDR_DSTATUS   (DDR_BASE + 0x0)
#define DDR_DCFG      (DDR_BASE + 0x4)
#define DDR_DCTRL     (DDR_BASE + 0x8)
#define DDR_DLMR      (DDR_BASE + 0xc)
#define DDR_DREFCNT   (DDR_BASE + 0x18)
#define DDR_DMMAP0    (DDR_BASE + 0x24)
#define DDR_DMMAP1    (DDR_BASE + 0x28)
#define DDR_DTIMING1  (DDR_BASE + 0x60)
#define DDR_DTIMING2  (DDR_BASE + 0x64)
#define DDR_DTIMING3  (DDR_BASE + 0x68)
#define DDR_DTIMING4  (DDR_BASE + 0x6c)
#define DDR_DTIMING5  (DDR_BASE + 0x70)
#define DDR_DTIMING6  (DDR_BASE + 0x74)
#define DDR_DREMAP1   (DDR_BASE + 0x9c)
#define DDR_DREMAP2   (DDR_BASE + 0xa0)
#define DDR_DREMAP3   (DDR_BASE + 0xa4)
#define DDR_DREMAP4   (DDR_BASE + 0xa8)
#define DDR_DREMAP5   (DDR_BASE + 0xac)
#define DDR_DDLP      (DDR_BASE + 0xbc)
#define DDR_WCMDCTRL1 (DDR_BASE + 0x100)
#define DDR_RCMDCTRL0 (DDR_BASE + 0x104)
#define DDR_RCMDCTRL1 (DDR_BASE + 0x108)
#define DDR_WDATTHD0  (DDR_BASE + 0x114)
#define DDR_WDATTHD1  (DDR_BASE + 0x118)
#define DDR_IPORTPRI  (DDR_BASE + 0x128)

/* DDR PHY */
#define DDRP_PIR    (DDRP_BASE + 0x04)
#define DDRP_PGCR   (DDRP_BASE + 0x08)
#define DDRP_PGSR   (DDRP_BASE + 0x0c)
#define DDRP_PTR0   (DDRP_BASE + 0x18)
#define DDRP_PTR1   (DDRP_BASE + 0x1c)
#define DDRP_PTR2   (DDRP_BASE + 0x20)
#define DDRP_ACIOCR (DDRP_BASE + 0x24)
#define DDRP_DXCCR  (DDRP_BASE + 0x28)
#define DDRP_DSGCR  (DDRP_BASE + 0x2c)
#define DDRP_DCR    (DDRP_BASE + 0x30)
#define DDRP_DTPR0  (DDRP_BASE + 0x34)
#define DDRP_DTPR1  (DDRP_BASE + 0x38)
#define DDRP_DTPR2  (DDRP_BASE + 0x3c)
#define DDRP_MR0    (DDRP_BASE + 0x40)
#define DDRP_MR1    (DDRP_BASE + 0x44)
#define DDRP_MR2    (DDRP_BASE + 0x48)
#define DDRP_MR3    (DDRP_BASE + 0x4c)
#define DDRP_ODTCR  (DDRP_BASE + 0x50)
#define DDRP_DTAR   (DDRP_BASE + 0x54)
#define DDRP_DTDR0  (DDRP_BASE + 0x58)
#define DDRP_DTDR1  (DDRP_BASE + 0x5c)
#define DDRP_DCUAR  (DDRP_BASE + 0xc0)
#define DDRP_DCUDR  (DDRP_BASE + 0xc4)
#define DDRP_DCURR  (DDRP_BASE + 0xc8)
#define DDRP_DCULR  (DDRP_BASE + 0xcc)
#define DDRP_DCUGCR (DDRP_BASE + 0xd0)
#define DDRP_DCUTPR (DDRP_BASE + 0xd4)
#define DDRP_DCUSR0 (DDRP_BASE + 0xd8)
#define DDRP_DCUSR1 (DDRP_BASE + 0xdc)
#define DDRP_ZQXCR0 (DDRP_BASE + 0x180)
#define DDRP_ZQXCR1 (DDRP_BASE + 0x184)
#define DDRP_DXGCR0 (DDRP_BASE + 0x1c0 + (0x40 * 0))
#define DDRP_DXGCR1 (DDRP_BASE + 0x1c0 + (0x40 * 1))
#define DDRP_DXGCR2 (DDRP_BASE + 0x1c0 + (0x40 * 2))
#define DDRP_DXGCR3 (DDRP_BASE + 0x1c0 + (0x40 * 3))
#define DDRP_DXGCR4 (DDRP_BASE + 0x1c0 + (0x40 * 4))
#define DDRP_DXGCR5 (DDRP_BASE + 0x1c0 + (0x40 * 5))
#define DDRP_DXGCR6 (DDRP_BASE + 0x1c0 + (0x40 * 6))
#define DDRP_DXGCR7 (DDRP_BASE + 0x1c0 + (0x40 * 7))

/* GPIOs */
#define GPIO_BANK_A 0x0
#define GPIO_BANK_B 0x100
#define GPIO_BANK_C 0x200
#define GPIO_BANK_D 0x300
#define GPIO_BANK_E 0x400
#define GPIO_BANK_F 0x500

#define GPIO_INTC   0x18
#define GPIO_MSKS   0x24
#define GPIO_MSKC   0x28
#define GPIO_PAT1S  0x34
#define GPIO_PAT1C  0x38
#define GPIO_PAT0S  0x44
#define GPIO_PAT0C  0x48
#define GPIO_PEC    0x78

#define GPIO(bank, name) (GPIO_BASE + (GPIO_BANK_##bank + GPIO_##name))

