ifndef APP
error "APP must be defined"
endif

__clean:
	rm -f ${OBJECTS} ${OBJDIR}/${APP}.elf ${OBJDIR}/${APP}.bin ${OBJDIR}/${APP}.srec
