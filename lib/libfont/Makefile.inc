LIBFONT =	osfive/lib/libfont/libfont.o

.for obj in ${LIBFONT}
CFLAGS_$(obj) =
.endfor

OBJECTS +=	${LIBFONT}
