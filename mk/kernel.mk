PYTHON ?= python3

ifeq (, $(shell which ${PYTHON}))
$(error "No ${PYTHON} found in PATH, consider setting PYTHON variable.")
endif
