KERN_OBJS =						\
		sys/kern/malloc_fl.o			\
		sys/kern/malloc.o			\
		sys/kern/mbuf.o				\
		sys/kern/mutex.o			\
		sys/kern/panic.o			\
		sys/kern/thread.o			\
		sys/kern/thread0.o			\
		sys/kern/sched.o			\
		sys/kern/sleep.o			\
		sys/kern/callout.o			\
		sys/kern/console.o			\
		sys/kern/printf.o			\
		sys/kern/startup.o			\

MIPS_OBJS =						\
		sys/mips/mips/cache_mipsNN.o		\
		sys/mips/mips/exception.o		\
		sys/mips/mips/machdep.o			\
		sys/mips/mips/saverestore.o		\
		sys/mips/mips/timer.o			\
		sys/mips/mips/trap.o			\

ARM_OBJS =						\
		sys/arm/arm/exception.o			\
		sys/arm/arm/machdep.o			\
		sys/arm/arm/nvic.o			\
		sys/arm/arm/scb.o			\
		sys/arm/arm/trap.o			\

RISCV_OBJS =						\
		sys/riscv/riscv/exception.o		\
		sys/riscv/riscv/intr.o			\
		sys/riscv/riscv/machdep.o		\
		sys/riscv/riscv/trap.o			\
