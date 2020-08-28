ENTRY(_start)

SECTIONS
{
	. = __BASE_ADDR__;
	.start . : {
		*start.o(.text)
	}

	.text : {
		*(.exception)
		*(.text)
	}

	.rodata : {
		*(.rodata)
	}

	/* Ensure _smem is associated with the next section */
	. = .;
	_smem = ABSOLUTE(.);
	.data : {
		_sdata = ABSOLUTE(.);
		*(.data)
		_edata = ABSOLUTE(.);
	}

	.bss : {
		_sbss = ABSOLUTE(.);
		*(.bss COMMON)
		_ebss = ABSOLUTE(.);
	}
}
