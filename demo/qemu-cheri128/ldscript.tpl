ENTRY(_start)

__size_boot_stack       = 0x10000;
CAP_ALIGN = 0x20;
PAGE_ALIGN = 0x1000;

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

	__start_bss = ADDR(.bss);
	__stop_bss = ALIGN(__start_bss + SIZEOF(.bss), CAP_ALIGN);
	__bss_size = __stop_bss - __start_bss;

        __start_boot_stack = ALIGN(__stop_bss, PAGE_ALIGN);
  	__stop_boot_stack  = __start_boot_stack + __size_boot_stack;

}
