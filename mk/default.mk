# Absolute path to osfive
OSDIR ?= ${CURDIR}/osfive

# Ensure OSDIR is absolute path
OSDIR := ${shell realpath ${OSDIR}}

# Object files directory
OBJDIR ?= obj

# Populate kernel and library objects first
include ${OSDIR}/mk/kernel.mk
include ${OSDIR}/mk/library.mk

include ${OSDIR}/mk/compile.mk
include ${OSDIR}/mk/machine.mk
include ${OSDIR}/mk/user.mk
