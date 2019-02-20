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
			    -I${CURDIR}/${OSDIR}/${inc}}		\
		)							\
))

OBJECTS := $(addprefix $(OBJDIR)/,${OBJECTS})

INCS += -I${CURDIR}
INCS += -I${CURDIR}/${OSDIR}/include
INCS += -I${CURDIR}/${OSDIR}/sys
INCS += -I${CURDIR}/${OSDIR}/lib

CFLAGS += ${INCS} ${CFLAGS_$@} -D__OSFIVE__

${OBJDIR}/%.o: %.c Makefile
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c -o $@ $<

${OBJDIR}/%.o: %.S Makefile
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c -o $@ $<

compile: objdir machine ${OBJECTS}
	@rm -f ${CURDIR}/machine

include ${OSDIR}/mk/gnu.objdir.mk
include ${OSDIR}/mk/gnu.machine.mk
