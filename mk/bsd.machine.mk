.ifndef MACHINE
.error "MACHINE must be defined"
.endif

.if !exists(${.CURDIR}/${OSDIR}/sys/${MACHINE}/include)
.error Error: machine headers not found.
.endif

__machine:
	@rm -f ${.OBJDIR}/machine
	@ln -s ${.CURDIR}/${OSDIR}/sys/${MACHINE}/include ${.OBJDIR}/machine
