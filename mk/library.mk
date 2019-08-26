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

LCMD = ${PYTHON} -B ${OSDIR}/tools/library.py ${MDX_CONFIG} ${OSDIR}
LIBS = $(shell ${LCMD} libs)

$(foreach lib, ${LIBS},							\
	${eval OPTS = $(shell ${LCMD} options ${lib})}			\
	$(foreach opt, ${OPTS},						\
		${eval LOBJS=$(shell ${LCMD} objs ${lib} ${opt})}	\
		${eval LINCS=$(shell ${LCMD} incs ${lib} ${opt})}	\
		$(foreach obj, ${LOBJS},				\
			${eval OBJECTS += ${OSDIR}/${obj}}		\
			$(foreach inc, ${LINCS},			\
				${eval CFLAGS_${OSOBJDIR}/${obj} +=	\
					-I${OSDIR}/${inc}}		\
			)						\
		)							\
	)								\
)
