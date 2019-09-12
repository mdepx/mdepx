APP = qemu-riscv64

OSDIR = ../../

export CFLAGS = -march=rv64imac -mabi=lp64 -mcmodel=medany		\
	-nostdinc -fno-builtin-printf -ffreestanding -Wall		\
	-Wredundant-decls -Wnested-externs -Wstrict-prototypes		\
	-Wmissing-prototypes -Wpointer-arith -Winline -Wcast-qual	\
	-Wundef -Wmissing-include-dirs -Werror

CMD = python3 -B ${OSDIR}/tools/emitter.py

all:
	${CMD} mdepx.conf

st:
	${CMD} mdepx-st.conf

up:
	${CMD} mdepx-up.conf

clean:
	@rm -rf obj/*

include ${OSDIR}/mk/user.mk
