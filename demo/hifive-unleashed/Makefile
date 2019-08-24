APP =		hifive-unleashed
MACHINE =	riscv

CC =		${CROSS_COMPILE}gcc
LD =		${CROSS_COMPILE}ld
OBJCOPY =	${CROSS_COMPILE}objcopy

OBJDIR =	obj
OSDIR = 	../../

LDSCRIPT =	${OBJDIR}/ldscript
LDSCRIPT_TPL =	${CURDIR}/ldscript.tpl

OBJECTS =	main.o						\
		${OSDIR}/kernel/riscv/sifive/e300g_clint.o	\
		${OSDIR}/kernel/riscv/sifive/e300g_uart.o	\
		start.o

define KERNEL_CONFIG
kernel {
	module callout;
	module malloc;
	module riscv;
	module sched;
	module systm;

	callout {
		include tsleep;
		include sched;
	};

	malloc {
		include fl;
		include fl_wrapper;
	};

	sched {
		include mutex;
		include malloc;
		include smp;

		nprio = 2;

		smp {
			maxcpu = 8;
		};
	};

	systm {
		include console;
	};
};
endef

LIBRARIES = libc

CFLAGS = -march=rv64imac -mabi=lp64 -mcmodel=medany			\
	-nostdinc -fno-builtin-printf -ffreestanding -Wall		\
	-Wredundant-decls -Wnested-externs -Wstrict-prototypes		\
	-Wmissing-prototypes -Wpointer-arith -Winline -Wcast-qual	\
	-Wundef -Wmissing-include-dirs -Werror

all:	${OBJDIR}/${APP}.bin

${LDSCRIPT}:
	@cat ${LDSCRIPT_TPL} > ${LDSCRIPT}

clean:
	@rm -f ${OBJECTS} ${LDSCRIPT} ${OBJDIR}/${APP}.*

include ${OSDIR}/lib/libc/Makefile.inc
include ${OSDIR}/mk/default.mk
