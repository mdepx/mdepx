all:
	make -C emul/qemu-riscv64 clean all run

purecap:
	make -C emul/qemu-riscv64c128 clean all run
