ifndef MACHINE
$(error Error: MACHINE must be defined)
endif

MACHINES = arm mips riscv

ifeq ($(filter $(MACHINE),$(MACHINES)),)
$(error Error: unknown arch ${MACHINE}. Available arches: ${MACHINES}.)
endif

__machine:
	@rm -f ${OBJDIR}/machine
	@ln -s ${CURDIR}/${OSDIR}/sys/${MACHINE}/include ${OBJDIR}/machine
