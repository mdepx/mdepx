.ifndef ARCH
.error "ARCH must be defined"
.endif

machine:
	@rm -f ${.CURDIR}/machine
	@ln -s ${.CURDIR}/osfive/sys/${ARCH}/include ${.CURDIR}/machine
