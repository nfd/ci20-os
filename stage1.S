#include "mipsregs.h"
#include "ci20board.h"

/* USB Bootloader */

.globl _start
/* Tell binutils it's a function */
.ent _start
.text

_start:
	move t0, ra
	move t1, sp

	li  sp, 0xf4004000

	jal   entrypoint

	move sp, t1
	move ra, t0

	j ra


.end _start

