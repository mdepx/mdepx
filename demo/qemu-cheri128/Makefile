APP = qemu-cheri128

OSDIR = ../../
OBJDIR = obj

BASE_ADDR =	0xffffffff80100000

LDSCRIPT_TPL =	${CURDIR}/ldscript.tpl
LDSCRIPT =	${OBJDIR}/ldscript

WARNFLAGS =			\
	-Werror			\
	-Wall			\
	-Wmissing-prototypes	\
	-Wredundant-decls	\
	-Wnested-externs	\
	-Wstrict-prototypes	\
	-Wmissing-prototypes	\
	-Wpointer-arith		\
	-Winline		\
	-Wcast-qual		\
	-Wundef			\
	-Wno-pointer-sign	\
	-Wmissing-include-dirs

COMMON_FLAGS = --target=cheri-unknown-freebsd -mcpu=cheri128		\
	-mabi=purecap -march=cheri128 -cheri-cap-table-abi=pcrel	\
	-ftls-model=local-exec -nostdinc -G0 -fno-builtin-printf	\
	-ffreestanding -msoft-float -fwrapv -fno-builtin-printf		\
	-fomit-frame-pointer						\
	-D__mips_n64 -nostdlib ${WARNFLAGS} -DBASE_ADDR=${BASE_ADDR}	\

export CFLAGS = ${COMMON_FLAGS} -fpic
export ASFLAGS = ${COMMON_FLAGS} -fno-pic

CMD = python3 -B ${OSDIR}/tools/emitter.py

all:	${LDSCRIPT}
	${CMD} mdepx.conf

${LDSCRIPT}:
	@sed s#__BASE_ADDR__#${BASE_ADDR}#g ${LDSCRIPT_TPL} > ${LDSCRIPT}

clean:
	@rm -rf obj/*

include ${OSDIR}/mk/user.mk
