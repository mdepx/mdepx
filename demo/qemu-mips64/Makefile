APP =		qemu-mips64
MACHINE =	mips

CC =		${CROSS_COMPILE}gcc
LD =		${CROSS_COMPILE}ld
OBJCOPY =	${CROSS_COMPILE}objcopy

OBJDIR =	obj
OSDIR =		../../

LDSCRIPT_TPL =	${CURDIR}/ldscript.tpl
LDSCRIPT =	${OBJDIR}/ldscript

BASE_ADDR =	0xffffffff80100000

OBJECTS =							\
	main.o							\
	start.o							\
	${OSDIR}/sys/dev/uart/uart_16550.o			\
	${OSDIR}/lib/libc/gen/assert.o

KERNEL = malloc sched (nprio=2) mutex		\
	 mips (cache tlb qemu)

LIBRARIES = libc

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

CFLAGS = -march=mips64 -mabi=64 -g -nostdinc -G0		\
	-nostdinc -fno-builtin-printf -ffreestanding		\
	-mno-abicalls -msoft-float -fwrapv -fno-builtin-printf	\
	-D__mips_n64 -nostdlib ${WARNFLAGS}			\
	-DBASE_ADDR=${BASE_ADDR}

all:	${OBJDIR}/${APP}.bin

${LDSCRIPT}:
	@sed s#__BASE_ADDR__#${BASE_ADDR}#g ${LDSCRIPT_TPL} > ${LDSCRIPT}

llvm-objdump:
	llvm-objdump-cheri -d ${APP}.elf | less

clean:
	@rm -f ${LDSCRIPT} ${OBJECTS}

include ${OSDIR}/lib/libc/Makefile.inc
include ${OSDIR}/mk/default.mk
