ifndef APP
$(error Error: APP must be defined)
endif

__clean:
	rm -f ${OBJECTS} ${OBJDIR}/machine ${OBJDIR}/${APP}.elf ${OBJDIR}/${APP}.bin ${OBJDIR}/${APP}.srec
