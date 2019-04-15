.ifndef MACHINE
.error "MACHINE must be defined"
.endif

MACHINES = arm mips riscv

.for arch in ${MACHINES}
.if ${arch} != ${MACHINE}
MACHINES_TMP += ${arch}
.endif
.endfor

.if "${MACHINES} == "${MACHINES_TMP}"
.error Error: unknown arch ${MACHINE}. Available arches: ${MACHINES}.
.endif

__machine:
	@rm -f ${.OBJDIR}/machine
	@ln -s ${.CURDIR}/${OSDIR}/sys/${MACHINE}/include ${.OBJDIR}/machine
