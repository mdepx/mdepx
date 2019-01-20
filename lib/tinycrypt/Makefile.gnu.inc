TINYCRYPT =	osfive/lib/tinycrypt/source/aes_decrypt.o	\
		osfive/lib/tinycrypt/source/aes_encrypt.o	\
		osfive/lib/tinycrypt/source/cbc_mode.o		\
		osfive/lib/tinycrypt/source/ccm_mode.o		\
		osfive/lib/tinycrypt/source/cmac_mode.o		\
		osfive/lib/tinycrypt/source/ctr_mode.o		\
		osfive/lib/tinycrypt/source/ctr_prng.o		\
		osfive/lib/tinycrypt/source/ecc.o		\
		osfive/lib/tinycrypt/source/ecc_dh.o		\
		osfive/lib/tinycrypt/source/ecc_dsa.o		\
		osfive/lib/tinycrypt/source/hmac.o		\
		osfive/lib/tinycrypt/source/hmac_prng.o		\
		osfive/lib/tinycrypt/source/sha256.o		\
		osfive/lib/tinycrypt/source/utils.o

TINYCRYPT_FLAGS = -I${CURDIR}/osfive/lib/tinycrypt/include	\
		-Wno-error=missing-prototypes			\
		-Wno-error=char-subscripts			\
		-std=c99

$(foreach obj,${TINYCRYPT},	\
	${eval CFLAGS_${OBJDIR}/$(obj) = ${TINYCRYPT_FLAGS}})

OBJECTS +=	${TINYCRYPT}
