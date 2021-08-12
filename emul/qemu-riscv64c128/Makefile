APP = qemu-riscv64c128

OSDIR = ../../
OBJDIR = obj

CROSS_COMPILE ?= riscv64-unknown-elf-
OBJCOPY = ${CROSS_COMPILE}objcopy

export CROSS_COMPILE

export CFLAGS = -march=rv64gc -mabi=lp64 -mcmodel=medany		\
	-nostdinc -fno-builtin-printf -ffreestanding -Wall		\
	-Wredundant-decls -Wnested-externs -Wstrict-prototypes		\
	-Wmissing-prototypes -Wpointer-arith -Winline -Wcast-qual	\
	-Wundef -Wmissing-include-dirs -Werror -mno-relax

export AFLAGS = ${CFLAGS}

CMD = python3 -B ${OSDIR}/tools/emitter.py

QEMU = ~/cheri/build/qemu-build/riscv64cheri-softmmu/qemu-system-riscv64cheri

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
	-device virtconsole,chardev=virtiocon0

all:
	@${CMD} -j mdepx.conf
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

include ${OSDIR}/mk/user.mk
