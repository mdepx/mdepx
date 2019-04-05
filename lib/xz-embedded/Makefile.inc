XZ_OBJS =	lib/xz-embedded/linux/lib/xz/xz_crc32.o		\
		lib/xz-embedded/linux/lib/xz/xz_dec_bcj.o	\
		lib/xz-embedded/linux/lib/xz/xz_dec_lzma2.o	\
		lib/xz-embedded/linux/lib/xz/xz_dec_stream.o

XZ_INCS =	lib/xz-embedded/linux/include/linux/		\
		lib/xz-embedded/

XZ_CFLAGS = 	-DXZ_DEC_ANY_CHECK=1
