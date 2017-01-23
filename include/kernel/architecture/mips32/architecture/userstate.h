/* Stores state which must be saved and restored when entering / exiting userspace.
 *
 * Field order must match assembly language.
*/
#include "inttypes.h"

struct architecture_userstate {
	uint32_t at;
	uint32_t v0;
	uint32_t v1;
	uint32_t a0;
	uint32_t a1;
	uint32_t a2;
	uint32_t a3;
	uint32_t t0;
	uint32_t t1;
	uint32_t t2;
	uint32_t t3;
	uint32_t t4;
	uint32_t t5;
	uint32_t t6;
	uint32_t t7;
	uint32_t s0;
	uint32_t s1;
	uint32_t s2;
	uint32_t s3;
	uint32_t s4;
	uint32_t s5;
	uint32_t s6;
	uint32_t s7;
	uint32_t t8;
	uint32_t t9;
	uint32_t gp;
	uint32_t fp;
	uint32_t ra;
	uint32_t sp;
	uint32_t cp0_EPC;
};

