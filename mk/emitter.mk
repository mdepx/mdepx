PYTHON ?= python3

ifeq (, $(shell which ${PYTHON}))
$(error "No ${PYTHON} found in PATH, consider setting PYTHON variable.")
endif

OSOBJDIR = ${OBJDIR}/${OSDIR}

LCMD = ${PYTHON} -B ${OSDIR}/tools/emitter.py
LIBS = $(shell ${LCMD} ${MDX_CONFIG} ${OBJDIR} ${OSDIR})
$(foreach lib, ${LIBS},			\
	${eval ${lib}}			\
);
