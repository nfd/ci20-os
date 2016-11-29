/*
Set up the PLLs and use them as clocks for the rest of the hardware.

There are 4 PLLs: APLL, MPLL, VPLL, and EPLL.

APLL is described as the "CPU PLL" in the PM, but in the reference
     implementation (and here) it's just set up to replicate EXTCLK.
MPLL is used for CPU and DDR.
VPLL is video.
EPLL is for "special purposes", which apparently includes audio.

A PLL takes an input clock and produces an output clock which is a multiple of
the input clock, where the multiplier is some value M divided by another value
N. The output of the PLL can be further divided by another value OD. These
there parameters (and a clock source, which is always the 48MHz EXTCLK on the
CI20) are enough to characterise each PLL.

The PLLs are set up with an M of twice what is required and an N or OD of 2.
Apparently this is a common practise in order to normalise the duty cycle,
reduce jitter, or because the PLL only works with an M of a certain range. It's
not clear which of these reasons apply to the JZ4780.
*/

#include "inttypes.h"
#include "ci20board.h"
#include "architecture/peekpoke.h"

/* Board-specific settings */

/* MPLL: run at 1.2 GHz */
#define MPLL_M (2 * (CPU_SPEED_HZ / EXTCLK))
#define MPLL_N 2
#define MPLL_OD 1

/* CPU and bus settings */
#define CI20_PDIV  (CPU_SPEED_HZ > 1000000000 ? 12 : 6)  /* Peripherals: 100MHz */
#define CI20_H2DIV  6  /* AHB2 bus: 200MHz */
#define CI20_H0DIV  3  /* AHB0 bus: 400MHz */
#define CI20_L2CDIV 2  /* L2 cache: 600MHz */
#define CI20_CDIV   1  /* CPU: 1.2GHz */

#define CI20_SEL_SRC   CPCCR_SEL_SRC_EXTCLK   
#define CI20_SEL_CPLL  CPCCR_SEL_CPLL_MPLL
#define CI20_SEL_H0PLL CPCCR_SEL_H0PLL_MPLL
#define CI20_SEL_H2PLL CPCCR_SEL_H2PLL_MPLL

/* DDR settings */
#define CI20_DDR_PLL   DDRCDR_DCS_MPLL               /* Use MPLL (1.2 GHz) ...*/
#define CI20_DDR_DIV   (CPU_SPEED_HZ / DDR_SPEED_HZ) /* ... divided by 3 to give 400MHz.*/

static void ddrclock_init(void)
{
	/* Set up the DDR PLL and divider, then enable it. */
	uint32_t val = CI20_DDR_PLL
		| ((CI20_DDR_DIV -1) << DDRCDR_CDR_SHIFT)
		| DDRCDR_CE_DDR;

	poke32(CPM_DDRCDR, val);

	/* Wait for frequency change to finish */
	while(peek32(CPM_DDRCDR) & DDRCDR_DDR_BUSY)
		;

	/* Enable the DDR clock */
	poke32(CPM_CLKGR0, peek32(CPM_CLKGR0) & ~CLKGR0_DDR0);
}

static void cpuclock_init(void)
{
	/* This is a two-stage process: first set dividers for the various
	 * peripherals, then set them up to use the newly-initialised PLLs.
	*/

	/* Step 1: dividers */
	/* ... read and clear all but the clock source bits... */
	uint32_t cpccr = peek32(CPM_CPCCR) & CPCCR_CLOCK_SRC_MASK;

	/* ... let me change the dividers... */
	cpccr |= CPCCR_CE_CPU | CPCCR_CE_AHB0 | CPCCR_CE_AHB2;

	/* ... set the dividiers for peripherals, AHB buses, cache, and CPU.. */
	cpccr |= ((CI20_PDIV -1) << CPCCR_PDIV_SHIFT);
	cpccr |= ((CI20_H2DIV -1)<< CPCCR_H2DIV_SHIFT);
	cpccr |= ((CI20_H0DIV -1)<< CPCCR_H0DIV_SHIFT);
	cpccr |= ((CI20_L2CDIV -1)<< CPCCR_L2CDIV_SHIFT);
	cpccr |= ((CI20_CDIV -1)<< CPCCR_CDIV_SHIFT);

	/* ... write the value... */
	poke32(CPM_CPCCR, cpccr);

	/* ... and wait for the new speeds to take effect. */
	while(peek32(CPM_CPCSR) & (CPCSR_H2DIV_BUSY | CPCSR_H0DIV_BUSY | CPCSR_CDIV_BUSY))
		;

	/* Step 2: clock sources */
	/* ... read and clear only the clock sources... */
	cpccr = peek32(CPM_CPCCR) & (~CPCCR_CLOCK_SRC_MASK);

	/* ... create the new value... */
	cpccr |= CI20_SEL_SRC;
	cpccr |= CI20_SEL_CPLL;
	cpccr |= CI20_SEL_H0PLL;
	cpccr |= CI20_SEL_H2PLL;

	/* ... and write. This should be a glitch-free transition. */
	poke32(CPM_CPCCR, cpccr);
}

static void pll_init(uint32_t pll, unsigned m, unsigned n, unsigned od)
{
	uint32_t val =
		((m - 1) << PLL_M_SHIFT)
		| ((n - 1) << PLL_N_SHIFT)
		| ((od - 1) << PLL_OD_SHIFT)
		| PLL_ENABLE_BIT;

	poke32(pll, val);

	/* Wait until the PLL has stabilised. */
	while((peek32(pll) & PLL_ON_BIT) == 0)
		;
}

/* Initialise PLLs and use them as clocks for parts of the system.  */
void pllclock_init(void)
{
	/* Set up PLLM, which is what we're going to use for CPU and DDR.
	 * Note that the boot ROM USB loader configures the CPU to use APLL,
	 * so we can't change it here: we have to configure it after we
	 * have moved the CPU over to using MPLL.
	*/
	pll_init(CPM_CPMPCR, MPLL_M, MPLL_N, MPLL_OD);

	/* Use a PLL for the CPU and associated peripherals */
	cpuclock_init();

	/* Use a PLL for the DDR controller */
	ddrclock_init();

	/* If we wanted to safely set up APLL, we could do it here. */
}

