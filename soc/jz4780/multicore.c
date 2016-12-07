#include "inttypes.h"

#include "soc/jz4780/jz47xx-cp0.h"
#include "soc/jz4780/jz47xx-cgm.h"
#include "soc/jz4780/jz47xx-ipi.h"
#include "driver/jz47xx-uart/jz47xx-uart.h"
#include "architecture/peekpoke.h"
#include "architecture/cpufunc.h"

void multicore_entrypoint_asm_stub(void);

void jz4780_enable_core1()
{
	/* Enabling the second core is xburst-specific. We also turn on
	 * inter-processor interrupts (IPIs), to allow for synchronisation between
	 * the two cores.
	*/

	/* Ungate CORE1's clock. CORE1 is the name used in the CPU section of the
	 * JZ4780 PM, but, confusingly, it's named both P1 and SCPU elsewhere
	 * (actually both names are used in the same module -- the CGM) */
	xburst_cgm_ungate_p1();

	// Clear pending IPIs
	uint32_t xburst_core_status = mips_read_cp0_xburst_core_status();
	xburst_core_status &= ~(CP0_CORE_STATUS_MIRQ1_P | CP0_CORE_STATUS_MIRQ0_P);
	mips_write_cp0_xburst_core_status(xburst_core_status);

	// Unmask IPI IRQs -- already done by our entry routine (but todo -- allowing all IRQs is not a great idea)
	
	// Allow IPIs and set new reset entrypoint address
	uint32_t core_reim = mips_read_cp0_xburst_core_reim() & ~CP0_CORE_REIM_ENTRY_MASK;
	core_reim |= MIPS_KSEG0_TO_KSEG1((uint32_t)(&multicore_entrypoint_asm_stub));
	core_reim |= (CP0_CORE_REIM_MIRQ0_M);
	core_reim |= (CP0_CORE_REIM_MIRQ1_M);
	mips_write_cp0_xburst_core_reim(core_reim);

	/* Put the core in reset and unmask its mailbox IRQ. */
	uint32_t core_ctl = mips_read_cp0_xburst_core_ctl();
	core_ctl |= CP0_CORE_CTL_SW_RST1;
	core_ctl |= CP0_CORE_CTL_RPC1;
	mips_write_cp0_xburst_core_ctl(core_ctl);

	/* Enable power to the core. */
	xburst_cgm_lpcr_pd_scpu_up();

	/* wait for the CPU to be powered up */
	while(xburst_cgm_lpcr_pd_scpu_is_down())
		;

	// Start the core.
	core_ctl &= ~(CP0_CORE_CTL_SW_RST1);
	mips_write_cp0_xburst_core_ctl(core_ctl);

}

