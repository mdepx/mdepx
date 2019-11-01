APP = qemu-mips64

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

export CFLAGS = -march=mips64 -mabi=64 -g -nostdinc -G0		\
	-nostdinc -fno-builtin-printf -ffreestanding		\
	-mno-abicalls -msoft-float -fwrapv -fno-builtin-printf	\
	-D__mips_n64 -nostdlib ${WARNFLAGS}			\
	-DBASE_ADDR=${BASE_ADDR}

export ASFLAGS = ${CFLAGS}

CMD = python3 -B ${OSDIR}/tools/emitter.py

all:	${LDSCRIPT}
	@${CMD} mdepx.conf

st:	${LDSCRIPT}
	@${CMD} mdepx-st.conf

${LDSCRIPT}:
	@sed s#__BASE_ADDR__#${BASE_ADDR}#g ${LDSCRIPT_TPL} > ${LDSCRIPT}

clean:
	rm -rf obj/*

include ${OSDIR}/mk/user.mk
