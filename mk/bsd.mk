OSDIR ?= osfive
OSOBJDIR = ${.OBJDIR}/${OSDIR}

.include "${.CURDIR}/${OSDIR}/mk/bsd.binutils.mk"
.include "${.CURDIR}/${OSDIR}/mk/bsd.clean.mk"
.include "${.CURDIR}/${OSDIR}/mk/bsd.compile.mk"
.include "${.CURDIR}/${OSDIR}/mk/bsd.info.mk"
.include "${.CURDIR}/${OSDIR}/mk/bsd.kernel.mk"
.include "${.CURDIR}/${OSDIR}/mk/bsd.library.mk"
.include "${.CURDIR}/${OSDIR}/mk/bsd.link.mk"
.include "${.CURDIR}/${OSDIR}/mk/bsd.user.mk"
