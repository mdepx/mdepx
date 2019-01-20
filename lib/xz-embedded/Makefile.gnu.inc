XZ_OBJECTS =	osfive/lib/xz-embedded/linux/lib/xz/xz_crc32.o		\
		osfive/lib/xz-embedded/linux/lib/xz/xz_dec_bcj.o	\
		osfive/lib/xz-embedded/linux/lib/xz/xz_dec_lzma2.o	\
		osfive/lib/xz-embedded/linux/lib/xz/xz_dec_stream.o

XZ_FLAGS = \
	-I${CURDIR}/osfive/lib/xz-embedded/linux/include/linux/		\
	-I${CURDIR}/osfive/lib/xz-embedded/				\
	-I${CURDIR}/machine/						\
	-DXZ_DEC_ANY_CHECK=1

$(foreach obj,${XZ_OBJECTS},	\
	${eval CFLAGS_${OBJDIR}/$(obj) = ${XZ_FLAGS}})

OBJECTS +=	${XZ_OBJECTS}
