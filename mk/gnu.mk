OSDIR ?= ${CURDIR}/osfive
OBJDIR ?= ${CURDIR}/obj
OSOBJDIR = ${OBJDIR}/${OSDIR}

include ${OSDIR}/lib/libaeabi/Makefile.inc
include ${OSDIR}/lib/libc/Makefile.inc
include ${OSDIR}/lib/libfdt/Makefile.inc
include ${OSDIR}/lib/libfont/Makefile.inc
include ${OSDIR}/lib/libFLAC/Makefile.inc
include ${OSDIR}/lib/mbedtls/Makefile.inc
include ${OSDIR}/lib/md5/Makefile.inc
include ${OSDIR}/lib/tinycrypt/Makefile.inc
include ${OSDIR}/lib/xz-embedded/Makefile.inc

include ${OSDIR}/mk/gnu.binutils.mk
include ${OSDIR}/mk/gnu.clean.mk
include ${OSDIR}/mk/gnu.compile.mk
include ${OSDIR}/mk/gnu.host.mk
include ${OSDIR}/mk/gnu.info.mk
include ${OSDIR}/mk/gnu.kernel.mk
include ${OSDIR}/mk/gnu.library.mk
include ${OSDIR}/mk/gnu.link.mk
include ${OSDIR}/mk/gnu.machine.mk
include ${OSDIR}/mk/gnu.user.mk
