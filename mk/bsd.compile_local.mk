.c.o .S.o:	Makefile
	@mkdir -p ${.TARGET:H}
	${CC} ${CFLAGS:M*} -c -o ${.TARGET} ${.IMPSRC}

compile: objdir ${OBJECTS}

.include "osfive/mk/bsd.objdir.mk"
