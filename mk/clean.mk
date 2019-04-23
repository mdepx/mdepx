ifndef APP
$(error Error: APP must be defined)
endif

_clean:
	@rm -f ${OBJECTS} ${OBJDIR}/machine ${OBJDIR}/${APP}.elf ${OBJDIR}/${APP}.bin ${OBJDIR}/${APP}.srec
