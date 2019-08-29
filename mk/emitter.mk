PYTHON ?= python3

ifeq (, $(shell which ${PYTHON}))
$(error "No ${PYTHON} found in PATH, consider setting PYTHON variable.")
endif

OSOBJDIR = ${OBJDIR}/${OSDIR}

ECMD = ${PYTHON} -B ${OSDIR}/tools/emitter.py
EOUTPUT = $(shell ${ECMD} ${MDX_CONFIG} ${OBJDIR} ${OSDIR})
$(foreach e, ${EOUTPUT},		\
	${eval ${e}}			\
);
