OBJECTS := $(addprefix $(OBJDIR)/,${OBJECTS})

${OBJDIR}/%.o: %.c Makefile
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} ${CFLAGS_$@} -c -o $@ $<

${OBJDIR}/%.o: %.S Makefile
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c -o $@ $<

compile: objdir machine ${OBJECTS}
	@rm -f ${CURDIR}/machine

include ${CURDIR}/osfive/mk/gnu.cflags.mk
include ${CURDIR}/osfive/mk/gnu.objdir.mk
include ${CURDIR}/osfive/mk/gnu.machine.mk
