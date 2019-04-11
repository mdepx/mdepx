OSDIR ?= osfive
OBJDIR ?= obj
OSOBJDIR = ${OBJDIR}/${OSDIR}

include ${CURDIR}/${OSDIR}/mk/gnu.binutils.mk
include ${CURDIR}/${OSDIR}/mk/gnu.clean.mk
include ${CURDIR}/${OSDIR}/mk/gnu.compile.mk
include ${CURDIR}/${OSDIR}/mk/gnu.info.mk
include ${CURDIR}/${OSDIR}/mk/gnu.library.mk
include ${CURDIR}/${OSDIR}/mk/gnu.link.mk
include ${CURDIR}/${OSDIR}/mk/gnu.user.mk
