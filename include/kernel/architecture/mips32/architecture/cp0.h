#ifdef __ASSEMBLER__
#define DOLLAR(x) $x
#else
#define DOLLAR(x) x
#endif

#define CP0_INDEX DOLLAR(0)
#define CP0_ENTRYLO0 DOLLAR(2)
#define CP0_ENTRYLO1 DOLLAR(3)
#define	CP0_TLB_PAGEMASK DOLLAR(5)
#define CP0_BADVADDR DOLLAR(8)
#define CP0_ENTRYHI DOLLAR(10)
#define CP0_STATUS DOLLAR(12)
#define CP0_CAUSE DOLLAR(13)
#define CP0_EPC DOLLAR(14)
#define CP0_CONFIG DOLLAR(16)
#define CP0_TAG_LO DOLLAR(28)
#define CP0_TAG_HI DOLLAR(29)

#define CP0_STATUS_CU_ALL 0xf0000000
#define CP0_STATUS_BEV (1 << 22)
#define CP0_STATUS_SR (1 << 20)
#define CP0_STATUS_KSU_USER (2 << 3)
#define CP0_STATUS_EXL (1 << 1)
#define CP0_STATUS_IM_ALL 0xff00
#define CP0_STATUS_IE 0x1

#define CP0_CAUSE_IV (1 << 23)
#define CP0_CAUSE_EXCCODE_MASK 0x7C
#define CP0_CAUSE_EXCCODE_SHIFT 2
#define CP0_CAUSE_EXCCODE_Sys 8			/* syscall */


