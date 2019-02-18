OSDIR := osfive
OBJDIR := obj
OSOBJDIR = ${OBJDIR}/${OSDIR}

$(foreach lib,${LIBRARIES},						\
	$(if $(filter %, ${${lib}_OBJECTS}),,				\
		$(error Error: library "${lib}" not found)		\
	endif)								\
	$(foreach obj,${${lib}_OBJECTS},				\
		${eval CFLAGS_${OSOBJDIR}/${obj} += ${${lib}_CFLAGS}}	\
		${eval CFLAGS_${OSOBJDIR}/${obj} += -I${CURDIR}/machine/}\
		${eval OBJECTS+=${OSDIR}/${obj}}			\
		$(foreach inc,${${lib}_INCS},				\
			${eval CFLAGS_${OSOBJDIR}/${obj} +=		\
			    -I${CURDIR}/osfive/${inc}}			\
		)							\
))

OBJECTS := $(addprefix $(OBJDIR)/,${OBJECTS})

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
