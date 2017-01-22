/* MIPS register definitions, O32 ABI */
/* See http://en.wikipedia.org/wiki/MIPS_instruction_set#Compiler_register_usage */

#if __ASSEMBLER__ == 1
#define DOLLAR(x) $x
#else
#define DOLLAR(x) x
#endif

#define zero DOLLAR(0)
#define _at DOLLAR(1) /* assembly temporary */
#define v0 DOLLAR(2) /* function return & expr eval */
#define v1 DOLLAR(3)
#define a0 DOLLAR(4) /* function arguments */
#define a1 DOLLAR(5)
#define a2 DOLLAR(6)
#define a3 DOLLAR(7)
#define t0 DOLLAR(8)
#define t1 DOLLAR(9)
#define t2 DOLLAR(10)
#define t3 DOLLAR(11)
#define t4 DOLLAR(12)
#define t5 DOLLAR(13)
#define t6 DOLLAR(14)
#define t7 DOLLAR(15)
#define s0 DOLLAR(16) /* callee-preserved temporary values */
#define s1 DOLLAR(17)
#define s2 DOLLAR(18)
#define s3 DOLLAR(19)
#define s4 DOLLAR(20)
#define s5 DOLLAR(21)
#define s6 DOLLAR(22)
#define s7 DOLLAR(23)
#define t8 DOLLAR(24) /* more temporary values */
#define t9 DOLLAR(25)
#define k0 DOLLAR(26) /* kernel only */
#define k1 DOLLAR(27)
#define gp DOLLAR(28) /* global pointer */
#define sp DOLLAR(29) /* stack pointer */
#define fp DOLLAR(30) /* frame pointer */
#define ra DOLLAR(31) /* return address */

