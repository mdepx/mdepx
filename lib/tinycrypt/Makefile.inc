TINYCRYPT_OBJS =					\
		lib/tinycrypt/source/aes_decrypt.o	\
		lib/tinycrypt/source/aes_encrypt.o	\
		lib/tinycrypt/source/cbc_mode.o		\
		lib/tinycrypt/source/ccm_mode.o		\
		lib/tinycrypt/source/cmac_mode.o	\
		lib/tinycrypt/source/ctr_mode.o		\
		lib/tinycrypt/source/ctr_prng.o		\
		lib/tinycrypt/source/ecc.o		\
		lib/tinycrypt/source/ecc_dh.o		\
		lib/tinycrypt/source/ecc_dsa.o		\
		lib/tinycrypt/source/hmac.o		\
		lib/tinycrypt/source/hmac_prng.o	\
		lib/tinycrypt/source/sha256.o		\
		lib/tinycrypt/source/utils.o

TINYCRYPT_INCS = lib/tinycrypt/include

TINYCRYPT_CFLAGS = 					\
		-Wno-error=missing-prototypes		\
		-Wno-error=char-subscripts		\
		-std=c99
