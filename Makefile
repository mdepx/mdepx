all:
	${MAKE} -C emul/qemu-riscv64 clean all run

purecap:
	${MAKE} -C emul/qemu-riscv64c128 clean all run
