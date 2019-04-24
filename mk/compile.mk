INCS += -I${OBJDIR}
INCS += -I${CURDIR}
INCS += -I${OSDIR}/include
INCS += -I${OSDIR}/sys
INCS += -I${OSDIR}/lib

CFLAGS += ${INCS} ${CFLAGS_$@} -D__OSFIVE__

#
# Add ${OBJDIR} prefix to each member of OBJECTS.
#
OBJECTS := $(addprefix ${OBJDIR}/, ${OBJECTS})

${OBJDIR}/%.o: %.c GNUmakefile
	@echo CC $(patsubst ${CURDIR}/%,%,$<)
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -c -o $@ $<

${OBJDIR}/%.o: %.c Makefile
	@echo CC $(patsubst ${CURDIR}/%,%,$<)
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -c -o $@ $<

${OBJDIR}/%.o: %.S GNUmakefile
	@echo CC $(patsubst ${CURDIR}/%,%,$<)
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -c -o $@ $<

${OBJDIR}/%.o: %.S Makefile
	@echo CC $(patsubst ${CURDIR}/%,%,$<)
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -c -o $@ $<

_compile: _machine ${sort ${OBJECTS}}
