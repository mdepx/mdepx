ifndef APP
$(error Error: APP must be defined)
endif

${OBJDIR}/${APP}.elf: ${LDSCRIPT} ${OBJECTS}
	@echo LD ${OBJDIR}/${APP}.elf
	@${LD} -T ${LDSCRIPT} ${OBJECTS} ${OBJECTS_LINK} -o ${OBJDIR}/${APP}.elf

${OBJDIR}/${APP}.bin: ${OBJDIR}/${APP}.elf
	${OBJCOPY} -O binary ${OBJDIR}/${APP}.elf ${OBJDIR}/${APP}.bin

${OBJDIR}/${APP}.srec: ${OBJDIR}/${APP}.elf
	${OBJCOPY} -O srec ${OBJDIR}/${APP}.elf ${OBJDIR}/${APP}.srec

_link:	${OBJDIR}/${APP}.elf
_binary: ${OBJDIR}/${APP}.bin
_srec:	${OBJDIR}/${APP}.srec
