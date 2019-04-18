ifndef MACHINE
$(error Error: MACHINE must be defined)
endif

ifeq ("$(wildcard ${OSDIR}/sys/${MACHINE}/include)","")
$(error Error: machine headers not found)
endif

__machine:
	@rm -f ${OBJDIR}/machine
	@mkdir -p ${OBJDIR}
	@ln -s ${OSDIR}/sys/${MACHINE}/include ${OBJDIR}/machine
