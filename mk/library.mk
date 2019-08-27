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
LIBS = $(shell ${LCMD} ${MDX_CONFIG} ${OBJDIR} ${OSDIR})
$(foreach lib, ${LIBS},			\
	${eval ${lib}}			\
);
