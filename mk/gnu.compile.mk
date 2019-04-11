INCS += -I${OBJDIR}
INCS += -I${CURDIR}
INCS += -I${CURDIR}/${OSDIR}/include
INCS += -I${CURDIR}/${OSDIR}/sys
INCS += -I${CURDIR}/${OSDIR}/lib

CFLAGS += ${INCS} ${CFLAGS_$@} -D__OSFIVE__

#
# Add ${OBJDIR} prefix to each member of OBJECTS.
# Since this file is loaded before gnu.library.mk
# the list does not include libraries objects.
#
OBJECTS := $(addprefix $(OBJDIR)/,${OBJECTS})

${OBJDIR}/%.o: %.c GNUmakefile
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c -o $@ $<

${OBJDIR}/%.o: %.S GNUmakefile
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c -o $@ $<

__compile: __objdir __machine ${OBJECTS}

include ${OSDIR}/mk/gnu.objdir.mk
include ${OSDIR}/mk/gnu.machine.mk
