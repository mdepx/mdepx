.ifndef APP
.error "APP must be defined"
.endif

clean:
	rm -f ${OBJECTS:M*} ${APP}.elf ${APP}.bin ${APP}.srec
