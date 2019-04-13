$(foreach lib0,${LIBRARIES},						\
	${eval lib=$(shell echo ${lib0} | tr '[:lower:]' '[:upper:]')}	\
	$(if $(filter %, ${${lib}_OBJS}),,				\
		$(error Error: library "${lib}" not found)		\
	endif)								\
	$(foreach obj,${${lib}_OBJS},					\
		${eval CFLAGS_${OSOBJDIR}/${obj} += ${${lib}_CFLAGS}}	\
		${eval CFLAGS_${OSOBJDIR}/${obj} += -I${OBJDIR}/machine/}\
		${eval OBJECTS+=${OSOBJDIR}/${obj}}			\
		$(foreach inc,${${lib}_INCS},				\
			${eval CFLAGS_${OSOBJDIR}/${obj} +=		\
			    -I${CURDIR}/${OSDIR}/${inc}}		\
		)							\
	)								\
)
