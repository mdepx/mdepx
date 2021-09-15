all:
	${MAKE} -C emul/qemu-riscv64 clean all run

supervisor:
	${MAKE} -C emul/qemu-riscv64 clean supervisor run-supervisor

purecap:
	${MAKE} -C emul/qemu-riscv64c128 clean all run
