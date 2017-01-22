#include "architecture/mipsregs.h"
#include "architecture/cp0.h"

.set noreorder

.globl jump_to_sigma0_asm
.ent jump_to_sigma0_asm

jump_to_sigma0_asm:
	/* first arg = core number (not changed) */
	/* second arg = entrypoint */
	mtc0 a1, CP0_EPC
	eret

.end jump_to_sigma0_asm
