PYTHON ?= python3

ifeq (, $(shell which ${PYTHON}))
$(error "No ${PYTHON} found in PATH, consider setting PYTHON variable.")
endif

KOBJECTS = $(shell ${PYTHON} -B ${OSDIR}/tools/kernel.py	\
	${OSDIR} "${KERNEL_CONFIG}")

$(foreach obj, ${KOBJECTS},					\
	${eval OBJECTS += ${OSDIR}/${obj}}			\
)

KFLAGS = $(shell ${PYTHON} -B ${OSDIR}/tools/flags.py		\
	"${KERNEL_CONFIG}")

$(foreach kflag, ${KFLAGS},					\
	${eval CFLAGS += -DMDX_${kflag}}			\
)
