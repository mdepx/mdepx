ifndef MACHINE
$(error Error: MACHINE must be defined)
endif

ifeq ("$(wildcard ${OSDIR}/sys/conf/files.${MACHINE})", "")
$(error Error: machine files not found)
endif

PYTHON ?= python3

ifeq (, $(shell which ${PYTHON}))
$(error "No ${PYTHON} found in PATH, consider setting PYTHON variable.")
endif

KOBJECTS = $(shell ${PYTHON} ${OSDIR}/tools/files.py		\
	${OSDIR} sys/conf/files.${MACHINE} ${KERNEL})

$(foreach obj, ${KOBJECTS},					\
	${eval OBJECTS += ${OSOBJDIR}/${obj}}			\
)
