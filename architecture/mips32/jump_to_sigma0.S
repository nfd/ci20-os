#include "architecture/mipsregs.h"
#include "architecture/cp0.h"

.set noreorder

.globl jump_to_sigma0_asm
.ent jump_to_sigma0_asm

jump_to_sigma0_asm:
	/* first arg = core number (not changed) */
	/* second arg = entrypoint */
	
	/* Turn off interrupts so we can manipulate EPC. They will be re-enabled by eret */
	mfc0 t0, CP0_STATUS
	ori t0, t0, CP0_STATUS_EXL
	mtc0 t0, CP0_STATUS
	nop

	/* Write our destination address */
	mtc0 a1, CP0_EPC
	nop

	/* Switch to user space, disable EXL, and re-enable interrupts. */
	ori t0, t0, CP0_STATUS_KSU_USER
	mtc0 t0, CP0_STATUS
	eret

.end jump_to_sigma0_asm
