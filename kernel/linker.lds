OUTPUT_ARCH(mips)

ENTRY(_start)

SECTIONS
{
	/* Our base address */
	. = 0x80000000;

	.text : {
		*(.start)
		*(.text)
	}

	/* Static data */
	.rodata ALIGN(0x20) : {
		*(.rodata)
		*(.rodata.*)
	}
	
	/* non-static data */
	.data ALIGN(0x20) : {
		*(.data*)
	}

	. = ALIGN(0x20);

	/* BSS -- define symbols for it so it can be cleared by start.S */
	__bss_start = .;
	.sbss (NOLOAD) : {
		*(.sbss*)
		*(.scommon)
	}

	.bss (NOLOAD) : {
		*(.bss*)
		*(COMMON)
	}
	__bss_end = .;
}

