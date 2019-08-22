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
	${OSDIR}/kernel/dev/uart/uart_16550.o			\
	${OSDIR}/lib/libc/gen/assert.o

define KERNEL_CONFIG
	module callout;
	module malloc;
	module mips;
	module systm;

	callout {
		include tsleep;
	};

	malloc {
		include fl;
		include fl_wrapper;
	};

	mips {
		include cache;
		include tlb;
		include qemu;
	};

	sched {
		include mutex;
		include malloc;

		nprio = 2;
	};

	systm {
		include console;
	};
endef

# Multi-threaded demo
ifndef MDX_ST
define KERNEL_CONFIG +=
	module sched;

	callout {
		include sched;
	};
endef
endif

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

test:
	echo $(shell python3 -B ${OSDIR}/tools/kernel.py \
		${OSDIR} "${KERNEL_CONFIG}")

clean:
	@rm -f ${LDSCRIPT} ${OBJECTS}

include ${OSDIR}/lib/libc/Makefile.inc
include ${OSDIR}/mk/default.mk
