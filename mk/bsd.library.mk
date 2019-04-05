.for lib0 in ${LIBRARIES}
lib=${lib0:tu}
.if empty(${lib}_OBJECTS)
.error Error: library "${lib}" not found
.endif
.for obj in ${${lib}_OBJECTS}
CFLAGS_${OSDIR}/${obj}+=${${lib}_CFLAGS}
CFLAGS_${OSDIR}/${obj}+=-I${.OBJDIR}/machine/
OBJECTS+=${OSDIR}/${obj}
.for inc in ${${lib}_INCS}
CFLAGS_${OSDIR}/${obj}+=-I${.CURDIR}/${OSDIR}/${inc}
.endfor
.endfor
.endfor
