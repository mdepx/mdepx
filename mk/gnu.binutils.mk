ifndef APP
$(error Error: APP must be defined)
endif

_objdump:
	${OBJDUMP} -d ${OBJDIR}/${APP}.elf | less

_readelf:
	${READELF} -a ${OBJDIR}/${APP}.elf | less
