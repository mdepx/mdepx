ifndef APP
error "APP must be defined"
endif

__objdump:
	${CROSS_COMPILE}objdump -d ${OBJDIR}/${APP}.elf ${ARGS} | less

__readelf:
	${CROSS_COMPILE}readelf -a ${OBJDIR}/${APP}.elf ${ARGS} | less
