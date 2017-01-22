OUTPUT_ARCH(mips)

ENTRY(_unicore_entrypoint_asm)

/* Ensure text, data, and BSS are in separate sections. Ld otherwise likes to
 * combine them, and Saruman / ElfIO can't generate segments where memsize !=
 * filesize (unless memsize == 0), so this results in larger-than-necessary
 * combined files.
*/
PHDRS
{
	text PT_LOAD;
	data PT_LOAD;
	bss PT_LOAD;
}

SECTIONS
{
	/* Our base address */
	. = 0x80000000;

	/* Lets us track the extent of the kernel in RAM for memory allocation */
	__kernel_start = .;

	.text : {
		*(.start)
		*(.text)
	} :text

	/* Static data */
	.rodata ALIGN(0x20) : {
		*(.rodata)
		*(.rodata.*)
	} :text
	
	/* non-static data */
	.data ALIGN(0x20) : {
		*(.data*)
	} :data

	. = ALIGN(0x20);

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

	__kernel_end = .;
}

