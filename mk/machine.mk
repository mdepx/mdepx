ifndef MACHINE
$(error Error: MACHINE must be defined)
endif

ifeq ("$(wildcard ${OSDIR}/kernel/${MACHINE}/include)","")
$(error Error: machine headers not found)
endif

${OBJDIR}/machine:
	@mkdir -p ${OBJDIR}
	@ln -sf ${OSDIR}/kernel/${MACHINE}/include ${OBJDIR}/machine
