INCS += -I${OBJDIR}
INCS += -I${CURDIR}
INCS += -I${OSDIR}/include
INCS += -I${OSDIR}/sys
INCS += -I${OSDIR}/lib

CFLAGS += ${INCS} ${CFLAGS_$@} -D__OSFIVE__

#
# Add ${OBJDIR} prefix to each member of OBJECTS.
# Note this is for objects provided by user only
# (gnu.library.mk and gnu.kernel.mk are loaded
# after this file)
#
OBJECTS := $(addprefix ${OBJDIR}/, ${OBJECTS})

${OBJDIR}/%.o: %.c GNUmakefile
	@echo CC $<
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -c -o $@ $<

${OBJDIR}/%.o: %.c Makefile
	@echo CC $<
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -c -o $@ $<

${OBJDIR}/%.o: %.S GNUmakefile
	@echo CC $<
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -c -o $@ $<

${OBJDIR}/%.o: %.S Makefile
	@echo CC $<
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -c -o $@ $<

_compile: _machine ${OBJECTS}
