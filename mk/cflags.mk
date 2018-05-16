CFLAGS += ${INCS} ${CFLAGS_$(.TARGET)} -D__OSFIVE__

INCS += -I${.CURDIR}
INCS += -I${.CURDIR}/osfive/sys
INCS += -I${.CURDIR}/osfive/lib
INCS += -I${.CURDIR}/osfive/lib/libc/include
