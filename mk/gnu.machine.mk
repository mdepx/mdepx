ifndef ARCH
$(error Error: ARCH must be defined)
endif

ARCHES = arm mips riscv

ifeq ($(filter $(ARCH),$(ARCHES)),)
$(error Error: unknown arch ${ARCH}. Available arches: ${ARCHES}.)
endif

__machine:
	@rm -f ${CURDIR}/machine
	@ln -s ${CURDIR}/${OSDIR}/sys/${ARCH}/include ${CURDIR}/machine
