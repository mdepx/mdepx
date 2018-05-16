.ifndef APP
.error "APP must be defined"
.endif

link:	${LDSCRIPT}
	${LD} -T ${LDSCRIPT} ${OBJECTS:M*} -o ${APP}.elf

binary:
	${OBJCOPY} -O binary ${APP}.elf ${APP}.bin

srec:
	${OBJCOPY} -O srec ${APP}.elf ${APP}.srec
