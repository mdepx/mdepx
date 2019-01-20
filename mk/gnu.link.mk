ifndef APP
error "APP must be defined"
endif

${OBJDIR}/${APP}.elf: ${LDSCRIPT} ${OBJECTS}
	${LD} -T ${LDSCRIPT} ${OBJECTS} -o ${OBJDIR}/${APP}.elf

${OBJDIR}/${APP}.bin: ${OBJDIR}/${APP}.elf
	${OBJCOPY} -O binary ${OBJDIR}/${APP}.elf ${OBJDIR}/${APP}.bin

${OBJDIR}/${APP}.srec: ${OBJDIR}/${APP}.elf
	${OBJCOPY} -O srec ${OBJDIR}/${APP}.elf ${OBJDIR}/${APP}.srec

link:	${OBJDIR}/${APP}.elf
binary: ${OBJDIR}/${APP}.bin
srec:	${OBJDIR}/${APP}.srec
