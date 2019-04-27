ifndef APP
$(error Error: APP must be defined)
endif

ifndef LDSCRIPT
$(error Error: LDSCRIPT must be defined)
endif

ifndef OBJDIR
$(error Error: OBJDIR must be defined)
endif

ifndef OSDIR
$(error Error: OSDIR must be defined)
endif

INCS += -I${OBJDIR}
INCS += -I${CURDIR}
INCS += -I${OSDIR}/include
INCS += -I${OSDIR}/sys
INCS += -I${OSDIR}/lib

CFLAGS += ${INCS} ${CFLAGS_$@} -D__OSFIVE__

#
# Add ${OBJDIR} prefix to each member of OBJECTS.
#
OBJECTS := $(addprefix ${OBJDIR}/, ${OBJECTS})

${OBJDIR}/%.o: %.c GNUmakefile
	@echo "  CC      $(patsubst ${CURDIR}/%,%,$<)"
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -c -o $@ $<

${OBJDIR}/%.o: %.c Makefile
	@echo "  CC      $(patsubst ${CURDIR}/%,%,$<)"
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -c -o $@ $<

${OBJDIR}/%.o: %.S GNUmakefile
	@echo "  CC      $(patsubst ${CURDIR}/%,%,$<)"
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -c -o $@ $<

${OBJDIR}/%.o: %.S Makefile
	@echo "  CC      $(patsubst ${CURDIR}/%,%,$<)"
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -c -o $@ $<

${OBJDIR}/${APP}.elf: ${OBJDIR}/machine ${LDSCRIPT} ${sort ${OBJECTS}}
	@echo "  LD      ${OBJDIR}/${APP}.elf"
	@${LD} -T ${LDSCRIPT} ${OBJECTS} ${OBJECTS_LINK}		\
		-o ${OBJDIR}/${APP}.elf
	@${SIZE} ${OBJDIR}/${APP}.elf

${OBJDIR}/${APP}.bin: ${OBJDIR}/${APP}.elf
	@echo "  OBJCOPY ${OBJDIR}/${APP}.bin"
	@${OBJCOPY} -O binary ${OBJDIR}/${APP}.elf ${OBJDIR}/${APP}.bin

${OBJDIR}/${APP}.srec: ${OBJDIR}/${APP}.elf
	@echo "  OBJCOPY ${OBJDIR}/${APP}.srec"
	@${OBJCOPY} -O srec ${OBJDIR}/${APP}.elf ${OBJDIR}/${APP}.srec
