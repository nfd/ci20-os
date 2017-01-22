#include "architecture/kernel_layout.h"

#define LAYOUT_PLACEHOLDER 0xcafed00d

struct architecture_memory_layout kernel_memory_layout = {
	.sigma0_entrypoint = (void *)LAYOUT_PLACEHOLDER,
	.kernel_start = (void *)LAYOUT_PLACEHOLDER,
	.kernel_end = (void *)LAYOUT_PLACEHOLDER
};



