.ifndef APP
.error "APP must be defined"
.endif

objdump:
	${CROSS_COMPILE}objdump -d device-model.elf | less
