KOBJECTS=$(shell ${PYTHON} ${CURDIR}/${OSDIR}/tools/files.py	\
	${CURDIR}/${OSDIR}/sys/conf/files ${KERNEL})

$(foreach obj, ${KOBJECTS},					\
	${eval OBJECTS+=${OSOBJDIR}/${obj}}			\
)
