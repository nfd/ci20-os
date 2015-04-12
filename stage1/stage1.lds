OUTPUT_ARCH(mips)

ENTRY(_start)

PHDRS {
        stage1 PT_LOAD;
}

SECTIONS
{
	/* Our base address */
	. = 0xf4000800;

	/* Code */
	.text : ALIGN(0x4) {
		*(.text)
	} : stage1

	/* Static data */
	.rodata : ALIGN(0x4) {
		*(.rodata)
		*(.rodata.*)
	} : stage1
	
	/* non-static data */
	.data : ALIGN(0x4) {
		*(.data*)
	} : stage1
}

