#include <inttypes.h>

struct architecture_memory_layout {
	void *sigma0_entrypoint;
	void *kernel_start;	/* first byte of kernel code (kseg0) */
	void *kernel_end;  /* last byte of kernel bss (kseg0) */
};

