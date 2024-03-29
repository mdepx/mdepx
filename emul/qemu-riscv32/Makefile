APP = qemu-riscv32

OSDIR = ../../
OBJDIR = obj

CROSS_COMPILE ?= riscv32-unknown-elf-
OBJCOPY = ${CROSS_COMPILE}objcopy

export CROSS_COMPILE

export CFLAGS = -march=rv32imc -mabi=ilp32 -mcmodel=medany		\
	-nostdinc -fno-builtin-printf -ffreestanding -Wall		\
	-Wredundant-decls -Wnested-externs -Wstrict-prototypes		\
	-Wmissing-prototypes -Wpointer-arith -Winline -Wcast-qual	\
	-Wundef -Wmissing-include-dirs -Werror -std=c99

export AFLAGS = ${CFLAGS}

BUILD_CMD = python3 -B ${OSDIR}/tools/emitter.py
QEMU = qemu-system-riscv32

RUN_CMD = ${QEMU}							\
	-smp cpus=1							\
	-machine virt							\
	-m 1024								\
	-kernel ${OBJDIR}/${APP}.elf					\
	-serial mon:stdio						\
	-bios none -nographic						\
	-device virtio-serial-device					\
	-nodefaults							\
	-drive if=none,id=sda,format=raw,file=${OBJDIR}/qemu.img	\
	-device virtio-blk-device,drive=sda				\
	-chardev socket,path=/tmp/virtio,server,nowait,id=virtiocon0	\
	-device virtconsole,chardev=virtiocon0				\
	-accel tcg,thread=multi

# Uncomment for debugging
# RUN_CMD += -D trace.log -d in_asm

# Uncomment for DTB
# RUN_CMD += -machine dumpdtb=dtb

# Multi-threaded, multi-processor
all:
	@${BUILD_CMD} -j mdepx.conf
	${CROSS_COMPILE}objcopy -O binary ${OBJDIR}/${APP}.elf	\
		${OBJDIR}/${APP}.bin

run:
	truncate -s 12m ${OBJDIR}/qemu.img
	${RUN_CMD}

dtb:
	${RUN_CMD} -machine dumpdtb=dtb.dtb
	dtc -I dtb -O dts dtb.dtb | less

clean:
	@rm -rf obj/*

objdump:
	${CROSS_COMPILE}objdump -d ${OBJDIR}/${APP}.elf | less

readelf:
	${CROSS_COMPILE}readelf -a ${OBJDIR}/${APP}.elf | less

include ${OSDIR}/mk/user.mk
