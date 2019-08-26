OSOBJDIR = ${OBJDIR}/${OSDIR}

$(foreach itm, ${LIBRARIES},						\
	${eval lib = $(shell echo ${itm} | tr '[:lower:]' '[:upper:]')}	\
	$(if $(filter %, ${${lib}_OBJS}),,				\
		$(error Error: library "${lib}" not found)		\
	endif)								\
	$(foreach obj, ${${lib}_OBJS},					\
		${eval CFLAGS_${OSOBJDIR}/${obj} += ${${lib}_CFLAGS}}	\
		${eval OBJECTS += ${OSDIR}/${obj}}			\
		$(foreach inc, ${${lib}_INCS},				\
			${eval CFLAGS_${OSOBJDIR}/${obj} +=		\
			    -I${OSDIR}/${inc}}				\
		)							\
	)								\
)

# New interface

LCMD = ${PYTHON} -B ${OSDIR}/tools/library.py
LIBS = $(shell ${LCMD} ${OSDIR} libs "${MDX_CONFIG}")

$(foreach lib, ${LIBS},							\
	${eval OPTS = $(shell ${LCMD}					\
		${OSDIR} options ${lib} "${MDX_CONFIG}")}		\
	$(foreach opt, ${OPTS},						\
		${eval LOBJS=$(shell ${LCMD}				\
			${OSDIR} objs ${lib} ${opt} "${MDX_CONFIG}")}	\
		${eval LINCS=$(shell ${LCMD}				\
			${OSDIR} incs ${lib} ${opt} "${MDX_CONFIG}")}	\
		$(foreach obj, ${LOBJS},				\
			${eval OBJECTS += ${OSDIR}/${obj}}		\
			$(foreach inc, ${LINCS},			\
				${eval CFLAGS_${OSOBJDIR}/${obj} +=	\
					-I${OSDIR}/${inc}}		\
			)						\
		)							\
	)								\
)
