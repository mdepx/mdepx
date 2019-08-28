OSOBJDIR = ${OBJDIR}/${OSDIR}

LCMD = ${PYTHON} -B ${OSDIR}/tools/emitter.py
LIBS = $(shell ${LCMD} ${MDX_CONFIG} ${OBJDIR} ${OSDIR})
$(foreach lib, ${LIBS},			\
	${eval ${lib}}			\
);
