KOBJECTS != ${PYTHON} ${.CURDIR}/${OSDIR}/tools/files.py	\
	${.CURDIR}/${OSDIR}/sys/conf/files ${KERNEL}

.for obj in ${KOBJECTS}
OBJECTS+=${OSDIR}/${obj}
.endfor
