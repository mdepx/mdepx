.ifndef APP
.error "APP must be defined"
.endif

__clean:
	rm -f ${OBJECTS:M*} ${APP}.elf ${APP}.bin ${APP}.srec
