OUTPUT_ARCH(mips)

ENTRY(_start)

SECTIONS
{
	/* Our base address */
	. = 0x80004000;

	/* Code */
	.text : {
		*(.text)
	}

	/* Static data */
	.rodata : {
		*(.rodata)
		*(.rodata.*)
	}

	/* TODO: "small" data -- gp relative? */
	/* See
	 * http://opencores.org/websvn,filedetails?repname=ion&path=%2Fion%2Ftrunk%2Fsrc%2Fion_noxram.lds
	 * for discussion on implications -- requires gp to be set, requires an
	 * sdata section (both of which should be doable) */
	
	/* non-static data */
	.data : {
		*(.data*)
	}

	/* BSS -- define symbols for it so it can be cleared by start.S */
	__bss_start = .;
	.sbss (NOLOAD) : {
		*(.sbss*)
	}

	.bss (NOLOAD) : {
		*(.bss*)
	}
	__bss_end = .;
}

