ifndef MACHINE
$(error Error: MACHINE must be defined)
endif

ifeq ("$(wildcard ${OSDIR}/sys/${MACHINE}/include)","")
$(error Error: machine headers not found)
endif

${OBJDIR}/machine:
	@mkdir -p ${OBJDIR}
	@ln -sf ${OSDIR}/sys/${MACHINE}/include ${OBJDIR}/machine
