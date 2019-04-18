OSDIR ?= ${CURDIR}/osfive
OBJDIR ?= ${CURDIR}/obj
OSOBJDIR = ${OBJDIR}/${OSDIR}

include ${OSDIR}/mk/gnu.binutils.mk
include ${OSDIR}/mk/gnu.clean.mk
include ${OSDIR}/mk/gnu.compile.mk
include ${OSDIR}/mk/gnu.info.mk
include ${OSDIR}/mk/gnu.kernel.mk
include ${OSDIR}/mk/gnu.library.mk
include ${OSDIR}/mk/gnu.link.mk
include ${OSDIR}/mk/gnu.machine.mk
include ${OSDIR}/mk/gnu.user.mk
