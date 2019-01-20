.c.o .S.o: Makefile
	@mkdir -p ${.TARGET:H}
	${CC} ${CFLAGS:M*} -c -o ${.TARGET} ${.IMPSRC}

compile: objdir machine ${OBJECTS}
	@rm -f ${.CURDIR}/machine

.include "osfive/mk/bsd.cflags.mk"
.include "osfive/mk/bsd.objdir.mk"
.include "osfive/mk/bsd.machine.mk"
