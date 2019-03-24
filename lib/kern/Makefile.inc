KERN_OBJECTS =						\
		sys/kern/kern_malloc_fl.o		\
		sys/kern/kern_malloc.o			\
		sys/kern/kern_mbuf.o			\
		sys/kern/kern_mutex.o			\
		sys/kern/kern_panic.o			\
		sys/kern/kern_sched.o			\
		sys/kern/kern_timeout.o			\
		sys/kern/subr_console.o			\
		sys/kern/subr_prf.o			\
		sys/kern/subr_sleep.o			\
		sys/kern/subr_startup.o			\

MIPS_OBJECTS =						\
		sys/mips/mips/cache_mipsNN.o		\
		sys/mips/mips/exception.o		\
		sys/mips/mips/machdep.o			\
		sys/mips/mips/saverestore.o		\
		sys/mips/mips/timer.o			\
		sys/mips/mips/trap.o			\

ARM_OBJECTS =						\
		sys/arm/arm/exception.o			\
		sys/arm/arm/machdep.o			\
		sys/arm/arm/nvic.o			\
		sys/arm/arm/scb.o			\
		sys/arm/arm/trap.o			\

RISCV_OBJECTS =						\
		sys/riscv/riscv/exception.o		\
		sys/riscv/riscv/intr.o			\
		sys/riscv/riscv/machdep.o		\
		sys/riscv/riscv/trap.o			\
