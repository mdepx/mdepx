.ifndef APP
.error "APP must be defined"
.endif

objdump:
	${CROSS_COMPILE}objdump -d ${APP}.elf | less

readelf:
	${CROSS_COMPILE}readelf -a ${APP}.elf | less
