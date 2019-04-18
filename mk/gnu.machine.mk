ifndef MACHINE
$(error Error: MACHINE must be defined)
endif

ifeq ("$(wildcard ${CURDIR}/${OSDIR}/sys/${MACHINE}/include)","")
$(error Error: machine headers not found)
endif

__machine:
	@rm -f ${OBJDIR}/machine
	@ln -s ${CURDIR}/${OSDIR}/sys/${MACHINE}/include ${OBJDIR}/machine
