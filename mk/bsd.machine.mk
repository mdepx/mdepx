.ifndef ARCH
.error "ARCH must be defined"
.endif

ARCHES = arm mips riscv

.for arch in ${ARCHES}
.if ${arch} != ${ARCH}
ARCHES_TMP += ${arch}
.endif
.endfor

.if "${ARCHES} == "${ARCHES_TMP}"
.error Error: unknown arch ${ARCH}. Available arches: ${ARCHES}.
.endif

__machine:
	@rm -f ${.OBJDIR}/machine
	@ln -s ${.CURDIR}/${OSDIR}/sys/${ARCH}/include ${.OBJDIR}/machine
