ifndef APP
error "APP must be defined"
endif

objdump:
	${CROSS_COMPILE}objdump -d ${OBJDIR}/${APP}.elf ${ARGS} | less

readelf:
	${CROSS_COMPILE}readelf -a ${OBJDIR}/${APP}.elf ${ARGS} | less
