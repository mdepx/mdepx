INCS += -I${.OBJDIR}
INCS += -I${.CURDIR}
INCS += -I${.CURDIR}/${OSDIR}/include
INCS += -I${.CURDIR}/${OSDIR}/sys
INCS += -I${.CURDIR}/${OSDIR}/lib

CFLAGS += ${INCS} ${CFLAGS_$(.TARGET)} -D__OSFIVE__

.c.o .S.o: Makefile
	@mkdir -p ${.TARGET:H}
	${CC} ${CFLAGS:M*} -c -o ${.TARGET} ${.IMPSRC}

__compile: __objdir __machine ${OBJECTS}

.include "${OSDIR}/mk/bsd.objdir.mk"
.include "${OSDIR}/mk/bsd.machine.mk"
