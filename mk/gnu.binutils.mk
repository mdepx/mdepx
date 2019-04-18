ifndef APP
$(error Error: APP must be defined)
endif

_objdump:
	${CROSS_COMPILE}objdump -d ${OBJDIR}/${APP}.elf | less

_readelf:
	${CROSS_COMPILE}readelf -a ${OBJDIR}/${APP}.elf | less
