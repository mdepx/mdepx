APP = qemu-riscv64

OSDIR = ../../
OBJDIR = obj

CROSS_COMPILE ?= riscv64-unknown-elf-
OBJCOPY = ${CROSS_COMPILE}objcopy
PYTHON = python3 -B

export CROSS_COMPILE

export CFLAGS = -march=rv64gc -mabi=lp64 -mcmodel=medany		\
	-nostdinc -fno-builtin-printf -ffreestanding -Wall		\
	-Wredundant-decls -Wnested-externs -Wstrict-prototypes		\
	-Wmissing-prototypes -Wpointer-arith -Winline -Wcast-qual	\
	-Wundef -Wmissing-include-dirs -Werror -std=c99

export AFLAGS = ${CFLAGS}

BUILD_CMD = ${PYTHON} ${OSDIR}/tools/emitter.py
NCPUS := $(shell ${PYTHON} ${OSDIR}/tools/ncpus.py -m 8)
QEMU = qemu-system-riscv64

RUN_CMD = ${QEMU}							\
	-smp cpus=${NCPUS}						\
	-machine virt							\
	-m 1024								\
	-kernel ${OBJDIR}/qemu-riscv64.elf				\
	-serial mon:stdio						\
	-bios none -nographic						\
	-device virtio-serial-device					\
	-nodefaults							\
	-drive if=none,id=sda,format=raw,file=${OBJDIR}/qemu.img	\
	-device virtio-blk-device,drive=sda				\
	-chardev socket,path=/tmp/virtio,server,nowait,id=virtiocon0	\
	-device virtconsole,chardev=virtiocon0				\
	-accel tcg,thread=multi

# Provide BBL elf for supervisor mode only
BBL ?= ~/cheri/build/bbl-baremetal-riscv64-build/bbl

# Uncomment for debugging
# RUN_CMD += -D trace.log -d in_asm,mmu

# Uncomment for DTB
# RUN_CMD += -machine dumpdtb=dtb

# Multi-threaded, multi-processor
all:
	@${BUILD_CMD} -j mdepx.conf
	${CROSS_COMPILE}objcopy -O binary ${OBJDIR}/${APP}.elf	\
		${OBJDIR}/${APP}.bin

# Single-threaded, uni-processor
st:
	@${BUILD_CMD} -j mdepx-singlethread.conf

# Multi-threaded, uni-processor
up:
	@${BUILD_CMD} -j mdepx-uniprocessor.conf

supervisor:
	@${BUILD_CMD} -j mdepx-supervisor.conf

run:
	truncate -s 12m ${OBJDIR}/qemu.img
	${RUN_CMD}

run-supervisor:
	truncate -s 12m ${OBJDIR}/qemu.img
	${RUN_CMD} -bios ${BBL}

dtb:	all
	truncate -s 12m ${OBJDIR}/qemu.img
	${RUN_CMD} -machine dumpdtb=obj/dtb.dtb
	dtc -I dtb -O dts obj/dtb.dtb | less

clean:
	@rm -rf obj/*

objdump:
	${CROSS_COMPILE}objdump -d ${OBJDIR}/${APP}.elf | less

readelf:
	${CROSS_COMPILE}readelf -a ${OBJDIR}/${APP}.elf | less

include ${OSDIR}/mk/user.mk
