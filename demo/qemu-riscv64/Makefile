APP =		qemu-riscv64
MACHINE =	riscv

CC =		${CROSS_COMPILE}gcc
LD =		${CROSS_COMPILE}ld
OBJCOPY =	${CROSS_COMPILE}objcopy

OBJDIR =	obj
OSDIR = 	../../

LDSCRIPT =	${OBJDIR}/ldscript
LDSCRIPT_TPL =	${CURDIR}/ldscript.tpl
HIFIVE1_FONT =	${CURDIR}/fonts/ter-124n.ld

OBJECTS =	main.o						\
		${OSDIR}/sys/dev/uart/uart_16550.o		\
		${OSDIR}/sys/riscv/sifive/e300g_clint.o		\
		${OSDIR}/sys/riscv/sifive/e300g_uart.o		\
		start.o

KERNEL = malloc sched mutex sem smp
LIBRARIES = libc

CFLAGS = -march=rv64imac -mabi=lp64 -mcmodel=medany
CFLAGS += -nostdinc -fno-builtin-printf -ffreestanding -Wall		\
	-Wredundant-decls -Wnested-externs -Wstrict-prototypes		\
	-Wmissing-prototypes -Wpointer-arith -Winline -Wcast-qual	\
	-Wundef -Wmissing-include-dirs -Werror				\
	-DCONFIG_NPRIO=2 -DMAXCPU=8

all:	${OBJDIR}/${APP}.bin

${LDSCRIPT}:
	@cat ${LDSCRIPT_TPL} > ${LDSCRIPT}

clean:
	@rm -f ${OBJECTS} ${LDSCRIPT} ${OBJDIR}/${APP}.*

include ${OSDIR}/lib/libc/Makefile.inc
include ${OSDIR}/mk/default.mk
