LIBC_OBJECTS =			\
		osfive/lib/libc/stdio/printf.o			\
		osfive/lib/libc/stdio/fprintf.o			\
		osfive/lib/libc/stdio/feof.o			\
		osfive/lib/libc/stdio/fopen.o			\
		osfive/lib/libc/stdio/fclose.o			\
		osfive/lib/libc/stdio/fread.o			\
		osfive/lib/libc/stdio/ferror.o			\
		osfive/lib/libc/stdio/fseek.o			\
		osfive/lib/libc/stdio/ftell.o			\
		osfive/lib/libc/stdio/fileno.o			\
		osfive/lib/libc/stdlib/qsort.o			\
		osfive/lib/libc/string/ffs.o			\
		osfive/lib/libc/string/ffsl.o			\
		osfive/lib/libc/string/ffsll.o			\
		osfive/lib/libc/string/fls.o			\
		osfive/lib/libc/string/flsl.o			\
		osfive/lib/libc/string/flsll.o			\
		osfive/lib/libc/string/strcasecmp.o		\
		osfive/lib/libc/string/strcat.o			\
		osfive/lib/libc/string/strcmp.o			\
		osfive/lib/libc/string/strlen.o			\
		osfive/lib/libc/string/strncat.o		\
		osfive/lib/libc/string/strncmp.o		\
		osfive/lib/libc/string/strsep.o			\
		osfive/lib/libc/string/strstr.o			\
		osfive/lib/libc/string/strtol.o			\
		osfive/lib/libc/string/memset.o			\
		osfive/lib/libc/string/memmove.o		\
		osfive/lib/libc/string/memchr.o			\
		osfive/lib/libc/string/memcmp.o			\
		osfive/lib/libc/string/bcopy.o			\
		osfive/lib/libc/string/bzero.o			\
		osfive/lib/libc/sys/fstat.o			\

LIBC_QUAD =			\
		osfive/lib/libc/quad/qdivrem.o			\
		osfive/lib/libc/quad/udivdi3.o			\
		osfive/lib/libc/quad/umoddi3.o			\

.for obj in ${LIBC_OBJECTS}
CFLAGS_$(obj) =
.endfor

OBJECTS +=	${LIBC_OBJECTS}
