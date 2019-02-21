ifndef APP
error "APP must be defined"
endif

${OBJDIR}/${APP}.elf: ${LDSCRIPT} ${OBJECTS}
	${LD} -T ${LDSCRIPT} ${OBJECTS} ${OBJECTS_LINK} -o ${OBJDIR}/${APP}.elf

${OBJDIR}/${APP}.bin: ${OBJDIR}/${APP}.elf
	${OBJCOPY} -O binary ${OBJDIR}/${APP}.elf ${OBJDIR}/${APP}.bin

${OBJDIR}/${APP}.srec: ${OBJDIR}/${APP}.elf
	${OBJCOPY} -O srec ${OBJDIR}/${APP}.elf ${OBJDIR}/${APP}.srec

__link:	${OBJDIR}/${APP}.elf
__binary: ${OBJDIR}/${APP}.bin
__srec:	${OBJDIR}/${APP}.srec
