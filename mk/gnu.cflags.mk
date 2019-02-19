CFLAGS += ${INCS} ${CFLAGS_$@} -D__OSFIVE__

INCS += -I${CURDIR}
INCS += -I${CURDIR}/${OSDIR}/include
INCS += -I${CURDIR}/${OSDIR}/sys
INCS += -I${CURDIR}/${OSDIR}/lib
