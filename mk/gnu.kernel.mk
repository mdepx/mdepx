PYTHON ?= python3

ifeq (, $(shell which ${PYTHON}))
$(error "No ${PYTHON} found in PATH, consider setting PYTHON variable.")
endif

KOBJECTS=$(shell ${PYTHON} ${CURDIR}/${OSDIR}/tools/files.py	\
	${CURDIR}/${OSDIR} sys/conf/files.${ARCH} ${KERNEL})

$(foreach obj, ${KOBJECTS},					\
	${eval OBJECTS+=${OSOBJDIR}/${obj}}			\
)
