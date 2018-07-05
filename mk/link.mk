.ifndef APP
.error "APP must be defined"
.endif

${APP}.elf: ${LDSCRIPT} ${OBJECTS}
	${LD} -T ${LDSCRIPT} ${OBJECTS:M*} -o ${APP}.elf

${APP}.bin: ${APP}.elf
	${OBJCOPY} -O binary ${APP}.elf ${APP}.bin

${APP}.srec: ${APP}.elf
	${OBJCOPY} -O srec ${APP}.elf ${APP}.srec

link:	${APP}.elf
binary: ${APP}.bin
srec:	${APP}.srec
