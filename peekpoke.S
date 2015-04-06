#include "mipsregs.h"

.set noreorder

.globl peek32
.ent peek32
peek32:
	lw v0, 0(a0)
	j ra
	nop
.end peek32

.globl poke32
.ent poke32
poke32:
	sw a1, (a0)
	j ra
	nop
.end poke32

