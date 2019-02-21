.for lib in ${LIBRARIES}
.if empty(${lib}_OBJECTS)
.error Error: library "${lib}" not found
.endif
.for obj in ${${lib}_OBJECTS}
CFLAGS_${OSDIR}/${obj}+=${${lib}_CFLAGS}
CFLAGS_${OSDIR}/${obj}+=-I${.CURDIR}/machine/
OBJECTS+=${OSDIR}/${obj}
.for inc in ${${lib}_INCS}
CFLAGS_${OSDIR}/${obj}+=-I${.CURDIR}/${OSDIR}/${inc}
.endfor
.endfor
.endfor

INCS += -I${.CURDIR}
INCS += -I${.CURDIR}/${OSDIR}/include
INCS += -I${.CURDIR}/${OSDIR}/sys
INCS += -I${.CURDIR}/${OSDIR}/lib

CFLAGS += ${INCS} ${CFLAGS_$(.TARGET)} -D__OSFIVE__

.c.o .S.o: Makefile
	@mkdir -p ${.TARGET:H}
	${CC} ${CFLAGS:M*} -c -o ${.TARGET} ${.IMPSRC}

__compile: __objdir __machine ${OBJECTS}
	@rm -f ${.CURDIR}/machine

.include "${OSDIR}/mk/bsd.objdir.mk"
.include "${OSDIR}/mk/bsd.machine.mk"
