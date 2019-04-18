$(foreach itm, ${LIBRARIES},						\
	${eval lib = $(shell echo ${itm} | tr '[:lower:]' '[:upper:]')}	\
	$(if $(filter %, ${${lib}_OBJS}),,				\
		$(error Error: library "${lib}" not found)		\
	endif)								\
	$(foreach obj,${${lib}_OBJS},					\
		${eval CFLAGS_${OSOBJDIR}/${obj} += ${${lib}_CFLAGS}}	\
		${eval OBJECTS+=${OSOBJDIR}/${obj}}			\
		$(foreach inc,${${lib}_INCS},				\
			${eval CFLAGS_${OSOBJDIR}/${obj} +=		\
			    -I${OSDIR}/${inc}}				\
		)							\
	)								\
)
