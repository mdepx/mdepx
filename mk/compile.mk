# Absolute path to osfive
OSDIR ?= ${CURDIR}/osfive

# Ensure OSDIR is absolute path
OSDIR := ${shell realpath ${OSDIR}}

# Object files directory
OBJDIR ?= obj

ifndef APP
$(error Error: APP must be defined)
endif

ifndef LDSCRIPT
$(error Error: LDSCRIPT must be defined)
endif

INCS += -I${OBJDIR}
INCS += -I${CURDIR}
INCS += -I${OSDIR}/include
INCS += -I${OSDIR}/sys
INCS += -I${OSDIR}/lib

CFLAGS += ${INCS} ${CFLAGS_$@} -D__OSFIVE__

#
# Populate kernel and library objects first
#
include ${OSDIR}/mk/kernel.mk
include ${OSDIR}/mk/library.mk

#
# Add ${OBJDIR} prefix to each member of OBJECTS.
#
OBJECTS := $(addprefix ${OBJDIR}/, ${OBJECTS})

include ${OSDIR}/mk/machine.mk
include ${OSDIR}/mk/objects.mk
include ${OSDIR}/mk/user.mk
