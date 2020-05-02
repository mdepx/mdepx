OUTPUT_ARCH("riscv")
ENTRY(_start)

MEMORY
{
	sram0 (wxa!ri)  : ORIGIN = 0x80000000, LENGTH = 6M /* cached */
	sram1 (wxa!ri)  : ORIGIN = 0x80100000, LENGTH = 5M /* cached */

	/* This region could be in use by some peripheral, have to check. */
	sram2 (wxa!ri)  : ORIGIN = 0x40000000, LENGTH = 6M
}

SECTIONS
{
	. = 0x80000000;

	.start . : {
		*start.o(.text)
	} > sram0

	.text : {
		*(.text)
	} > sram0

	.rodata : {
		*(.rodata)
	} > sram0

	/* Ensure _smem is associated with the next section */
	. = .;
	_smem = ABSOLUTE(.);
	.sdata : {
		_sdata = ABSOLUTE(.);
		*(.sdata)
		_edata = ABSOLUTE(.);
	} > sram0

	.bss : {
		_sbss = ABSOLUTE(.);
		*(.bss COMMON)
		*(.sbss)
		_ebss = ABSOLUTE(.);
	} > sram0
}
