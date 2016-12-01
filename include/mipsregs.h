/* MIPS register definitions, O32 ABI */
/* See http://en.wikipedia.org/wiki/MIPS_instruction_set#Compiler_register_usage */

#define zero $0
#define _at $1 /* assembly temporary */
#define v0 $2 /* function return & expr eval */
#define v1 $3
#define a0 $4 /* function arguments */
#define a1 $5
#define a2 $6
#define a3 $7
#define t0 $8  /* temporary values */
#define t1 $9
#define t2 $10
#define t3 $11
#define t4 $12
#define t5 $13
#define t6 $14
#define t7 $15
#define s0 $16 /* callee-preserved temporary values */
#define s1 $17
#define s2 $18
#define s3 $19
#define s4 $20
#define s5 $21
#define s6 $22
#define s7 $23
#define t8 $24 /* more temporary values */
#define t9 $25
#define k0 $26 /* kernel only */
#define k1 $27
#define gp $28 /* global pointer */
#define sp $29 /* stack pointer */
#define fp $30 /* frame pointer */
#define ra $31 /* return address */

#define CP0_STATUS $12
#define CP0_CAUSE $13
#define CP0_EPC $14
#define CP0_CONFIG $16

#define CACHE_MODE_CACHABLE_NONCOHERENT 3
#define CP0_STATUS_CU_ALL 0xf0000000
#define CP0_STATUS_IM_ALL 0xff00
#define CP0_STATUS_IE 0x1

