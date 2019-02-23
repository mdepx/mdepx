ifndef APP
error "APP must be defined"
endif

__o:
	${CROSS_COMPILE}objdump -d ${OBJDIR}/${APP}.elf ${ARGS} | less

__r:
	${CROSS_COMPILE}readelf -a ${OBJDIR}/${APP}.elf ${ARGS} | less
