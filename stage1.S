#include "mipsregs.h"
#include "ci20board.h"

/* USB Bootloader */

.globl _start
/* Tell binutils it's a function */
.ent _start
.text

_start:
	/* Enable caching in kseg0 */
	li      t0, CACHE_MODE_CACHABLE_NONCOHERENT
	mtc0    t0, CP0_CONFIG
	nop

	li  sp, 0xf4004000

	jal   entrypoint

1:
	j 1b

.end _start

