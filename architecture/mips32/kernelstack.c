#include <inttypes.h>
#include <soc/config.h>
#include <architecture/kernelstack.h>
#include <architecture/memory.h>

uint8_t kernel_stack[NUM_CORES * KERNEL_STACK_SIZE] __attribute__ ((aligned (PAGE_SIZE))) ;

