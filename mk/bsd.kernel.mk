.ifndef MACHINE
.error "MACHINE must be defined"
.endif

.if !exists(${.CURDIR}/${OSDIR}/sys/conf/files.${MACHINE})
.error Error: machine files not found.
.endif

PYTHON ?= python3
PYTHON_PATH != which ${PYTHON}

.if "${PYTHON_PATH}" == ""
.error "No ${PYTHON} found in PATH, consider setting PYTHON variable."
.endif

KOBJECTS != ${PYTHON} ${.CURDIR}/${OSDIR}/tools/files.py	\
	${.CURDIR}/${OSDIR} sys/conf/files.${MACHINE} ${KERNEL}

.for obj in ${KOBJECTS}
OBJECTS+=${OSDIR}/${obj}
.endfor
