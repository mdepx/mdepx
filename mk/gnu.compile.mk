OBJECTS := $(addprefix $(OBJDIR)/,${OBJECTS})

$(foreach lib,${LIBRARIES},						\
$(foreach obj,${${lib}_OBJECTS},					\
	${eval CFLAGS_osfive/${obj} += ${${lib}_CFLAGS}};		\
	${eval CFLAGS_osfive/${obj} += -I${CURDIR}/machine/};		\
	${eval OBJECTS+=osfive/${obj}};					\
	$(foreach inc,${${lib}_INCS},					\
		${eval CFLAGS_osfive/${obj} +=				\
		    -I${CURDIR}/osfive/${inc}};				\
	)								\
))

${OBJDIR}/%.o: %.c Makefile
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c -o $@ $<

${OBJDIR}/%.o: %.S Makefile
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c -o $@ $<

compile: objdir machine ${OBJECTS}
	@rm -f ${CURDIR}/machine

include ${CURDIR}/osfive/mk/gnu.cflags.mk
include ${CURDIR}/osfive/mk/gnu.objdir.mk
include ${CURDIR}/osfive/mk/gnu.machine.mk
