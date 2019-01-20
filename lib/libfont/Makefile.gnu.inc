LIBFONT =	osfive/lib/libfont/libfont.o

$(foreach obj,${LIBFONT},	\
	${eval CFLAGS_${OBJDIR}/$(obj) = })

OBJECTS +=	${LIBFONT}
