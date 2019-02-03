.for lib in ${LIBRARIES}
.for obj in ${${lib}_OBJECTS}
CFLAGS_osfive/${obj}+=${${lib}_CFLAGS}
CFLAGS_osfive/${obj}+=-I${.CURDIR}/machine/
OBJECTS+=osfive/${obj}
.for inc in ${${lib}_INCS}
CFLAGS_osfive/${obj}+=-I${.CURDIR}/osfive/${inc}
.endfor
.endfor
.endfor

.c.o .S.o: Makefile
	@mkdir -p ${.TARGET:H}
	${CC} ${CFLAGS:M*} -c -o ${.TARGET} ${.IMPSRC}

compile: objdir machine ${OBJECTS}
	@rm -f ${.CURDIR}/machine

.include "osfive/mk/bsd.cflags.mk"
.include "osfive/mk/bsd.objdir.mk"
.include "osfive/mk/bsd.machine.mk"
