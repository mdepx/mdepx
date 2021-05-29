APP = qemu-riscv64

OSDIR = ../../

export CFLAGS = -march=rv64gc -mabi=lp64 -mcmodel=medany		\
	-nostdinc -fno-builtin-printf -ffreestanding -Wall		\
	-Wredundant-decls -Wnested-externs -Wstrict-prototypes		\
	-Wmissing-prototypes -Wpointer-arith -Winline -Wcast-qual	\
	-Wundef -Wmissing-include-dirs -Werror

export AFLAGS = ${CFLAGS}

CMD = python3 -B ${OSDIR}/tools/emitter.py

RUN_CMD = qemu-system-riscv64						\
	-smp cpus=8							\
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
	-device virtconsole,chardev=virtiocon0

run:
	truncate -s 12m ${OBJDIR}/qemu.img
	${RUN_CMD}

dtb:
	${RUN_CMD} -machine dumpdtb=dtb.dtb
	dtc -I dtb -O dts dtb.dtb | less

# Multi-threaded, multi-processor
all:
	@${CMD} -j mdepx.conf

# Single-threaded, uni-processor
st:
	@${CMD} -j mdepx-st.conf

# Multi-threaded, uni-processor
up:
	@${CMD} -j mdepx-up.conf

clean:
	@rm -rf obj/*

include ${OSDIR}/mk/user.mk
