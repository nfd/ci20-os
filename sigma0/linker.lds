OUTPUT_ARCH(mips)

ENTRY(_start)

PHDRS
{
	text PT_LOAD;
	data PT_LOAD;
	bss PT_LOAD;
}

/* Our base address */
INCLUDE base_vaddr.lds

SECTIONS
{
	__sigma0_start = .;

	.text : {
		*(.start)
		*(.text)
	} :text

	. = ALIGN(0x1000);

	/* Static data */
	.rodata ALIGN(0x20) : {
		*(.rodata)
		*(.rodata.*)
	} :text
	
	/* non-static data */
	.data ALIGN(0x20) : {
		*(.data*)
	} :data

	. = ALIGN(0x1000);

	/* BSS -- define symbols for it so it can be cleared by start.S */
	__bss_start = .;
	.sbss (NOLOAD) : {
		*(.sbss*)
		*(.scommon)
	} :bss

	.bss (NOLOAD) : {
		*(.bss*)
		*(COMMON)
	} :bss
	__bss_end = .;

	__sigma0_end = .;
}

