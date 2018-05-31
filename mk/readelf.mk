.ifndef APP
.error "APP must be defined"
.endif

readelf:
	${CROSS_COMPILE}readelf -a ${APP}.elf | less
