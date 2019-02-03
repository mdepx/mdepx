LIBFLAC_OBJECTS =					\
		lib/libFLAC/libFLAC/bitmath.o		\
		lib/libFLAC/libFLAC/bitreader.o		\
		lib/libFLAC/libFLAC/crc.o		\
		lib/libFLAC/libFLAC/format.o		\
		lib/libFLAC/libFLAC/lpc.o		\
		lib/libFLAC/libFLAC/cpu.o		\
		lib/libFLAC/libFLAC/md5.o		\
		lib/libFLAC/libFLAC/float.o		\
		lib/libFLAC/libFLAC/fixed.o		\
		lib/libFLAC/libFLAC/memory.o		\
		lib/libFLAC/libFLAC/stream_decoder.o

LIBFLAC_INCS =						\
		lib/libFLAC/include			\
		lib/libFLAC/libFLAC/include

LIBFLAC_CFLAGS = -DHAVE_CONFIG_H
