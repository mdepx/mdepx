APP =		qemu-riscv64
MACHINE =	riscv

CC =		${CROSS_COMPILE}gcc
LD =		${CROSS_COMPILE}ld
OBJCOPY =	${CROSS_COMPILE}objcopy

OBJDIR =	obj
OSDIR = 	../../

LDSCRIPT =	${OBJDIR}/ldscript
LDSCRIPT_TPL =	${CURDIR}/ldscript.tpl

OBJECTS =	main.o						\
		${OSDIR}/sys/dev/uart/uart_16550.o		\
		${OSDIR}/sys/riscv/sifive/e300g_clint.o		\
		${OSDIR}/sys/riscv/sifive/e300g_uart.o		\
		start.o

KERNEL =						\
	riscv						\
	malloc (fl fl_wrapper)				\
	sched (nprio=2 mutex smp [maxcpu=8] malloc)	\
	callout (tsleep sched)				\
	systm (console)

# Uni-processor demo
ifdef MDX_UP
KERNEL =				\
	riscv				\
	malloc (fl fl_wrapper)		\
	sched (nprio=2 mutex malloc)	\
	callout (tsleep sched)		\
	systm (console)
endif

# Single-threaded demo
ifdef MDX_ST
KERNEL = malloc
endif

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
