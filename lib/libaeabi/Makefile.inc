LIBAEABI =	\
	osfive/lib/libaeabi/crt.o	\
	osfive/lib/libaeabi/divdi3.o	\
	osfive/lib/libaeabi/idiv.o	\
	osfive/lib/libaeabi/idivmod.o	\
	osfive/lib/libaeabi/lasr.o	\
	osfive/lib/libaeabi/lcmp.o	\
	osfive/lib/libaeabi/ldivmod.o	\
	osfive/lib/libaeabi/lmul.o	\
	osfive/lib/libaeabi/memmove.o	\
	osfive/lib/libaeabi/memset.o	\
	osfive/lib/libaeabi/moddi3.o	\
	osfive/lib/libaeabi/modsi3.o	\
	osfive/lib/libaeabi/ulcmp.o	\
	osfive/lib/libaeabi/umoddi3.o	\
	osfive/lib/libaeabi/umodsi3.o	\
	osfive/lib/libaeabi/uread4.o	\
	osfive/lib/libaeabi/uread8.o	\
	osfive/lib/libaeabi/uwrite4.o	\
	osfive/lib/libaeabi/uwrite8.o

UNUSED = 	\
	osfive/lib/libaeabi/llsl.o	\
	osfive/lib/libaeabi/llsr.o

OBJECTS += ${LIBAEABI}
