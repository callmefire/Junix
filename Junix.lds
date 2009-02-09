OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(phy_start)

SECTIONS
{
	. =  0xC0100000 ;
	phy_start = _start - 0xC0000000;
	.text : AT(ADDR(.text) - 0xC0000000)	{
		_text = .; 	/* Text */
		*(.text)
		*(.text.*)
		_etext = . ;
	}
	.rodata : AT(ADDR(.rodata) - 0xC0000000) {
		_rodata = . ;
		*(.rodata)	 /* read-only data */
		*(.rodata.*)
		_erodata = . ;
	}
	.data : AT(ADDR(.data) - 0xC0000000)	{
		_data = . ;
		*(.data)
		*(.data.*)
		_edata = . ;
	}
	.bss : AT(ADDR(.bss) - 0xC0000000) {
		_bss = . ;
		*(.bss)
		*(.bss.*)
		*(COMMON)
		_end = . ;
		. = ALIGN(4096);
		pg0 = .;
	}
}

