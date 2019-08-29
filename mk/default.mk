# Absolute path to mdepx
OSDIR ?= ${CURDIR}/mdepx

# Ensure OSDIR is absolute path
OSDIR := ${shell realpath ${OSDIR}}

# Object files directory
OBJDIR ?= obj

# Check if application name is set
ifndef APP
$(error Error: APP must be defined)
endif

ifndef LDSCRIPT
$(error Error: LDSCRIPT must be defined)
endif

INCS += -I${OBJDIR}
INCS += -I${CURDIR}
INCS += -I${OSDIR}/include
INCS += -I${OSDIR}/kernel
INCS += -I${OSDIR}/lib

CFLAGS += ${INCS} ${CFLAGS_$@} -D__MDX__

#
# Populate kernel and library objects first.
#
MDX_CONFIG := ${CURDIR}/config.mdx
include ${OSDIR}/mk/emitter.mk

#
# Add ${OBJDIR} prefix to each member of OBJECTS.
#
OBJECTS := $(addprefix ${OBJDIR}/, ${OBJECTS})

#
# Check if user has provided CROSS_COMPILE variable
# or any extra targets for this app.
#
include ${OSDIR}/mk/user.mk

#
# Include standard build targets.
#
include ${OSDIR}/mk/machine.mk
include ${OSDIR}/mk/objects.mk
