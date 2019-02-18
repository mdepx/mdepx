MBEDTLS_MDSHA_OBJECTS = 		\
	lib/mbedtls/library/md_wrap.o	\
	lib/mbedtls/library/md.o	\
	lib/mbedtls/library/md2.o	\
	lib/mbedtls/library/md4.o	\
	lib/mbedtls/library/md5.o	\
	lib/mbedtls/library/sha1.o	\
	lib/mbedtls/library/sha256.o	\
	lib/mbedtls/library/sha512.o	\
	lib/mbedtls/library/ripemd160.o

MBEDTLS_MDSHA_CFLAGS = -I${.CURDIR}/osfive/lib/mbedtls/include/
