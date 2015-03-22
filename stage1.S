#include "mipsregs.h"
#include "ci20board.h"

/* USB Bootloader */

.globl _start
/* Tell binutils it's a function */
.ent _start
.text

_start:
	li  sp, 0xf4004000

	jal   entrypoint

	j ra

1:
	j 1b

.end _start

