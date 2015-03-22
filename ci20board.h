#define EXTCLK 48000000 /* 48 MHz external clock driving the serial line */

/* CPU and DDR will be configured to use the same clock, so CPU speed divided
 * by DDR speed must be an integer. */
#define CPU_SPEED_HZ     1200000000 /* 1.2 GHz */
#define DDR_SPEED_HZ      400000000 /* 400 MHz */

/* Devices */
#define CPM_BASE   0xb0000000
#define GPIO_BASE  0xb0010000
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

/* Affects DDRCDR */
#define DDRCDR_DCS_STOP    (0 << 30)
#define DDRCDR_DCS_SCLK_A  (1 << 30)
#define DDRCDR_DCS_MPLL    (2 << 30)
#define DDRCDR_CE_DDR      (1 << 29)
#define DDRCDR_DDR_BUSY    (1 << 28)
#define DDRCDR_DDR_STOP    (1 << 27)
#define DDRCDR_CDR_SHIFT   0

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

/* Pin names: GPIO bank C, mode 2 */
#define PIN_UART4_TXD (1 << 10)
#define PIN_UART4_RXD (1 << 20)

/* Pin names: GPIO bank F */
#define PIN_LED_PIN   (1 << 15)

