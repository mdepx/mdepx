ifdef CROSS_COMPILE
LD		= ${CROSS_COMPILE}ld
CC		= ${CROSS_COMPILE}gcc
AR		= ${CROSS_COMPILE}ar
NM		= ${CROSS_COMPILE}nm
STRIP		= ${CROSS_COMPILE}strip
OBJCOPY		= ${CROSS_COMPILE}objcopy
OBJDUMP		= ${CROSS_COMPILE}objdump
READELF		= ${CROSS_COMPILE}readelf
SIZE		= ${CROSS_COMPILE}size
endif

${OBJDIR}/%.o: %.c GNUmakefile
	@echo "  CC      $(patsubst ${CURDIR}/%,%,$<)"
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -c -o $@ $<

${OBJDIR}/%.o: %.c Makefile
	@echo "  CC      $(patsubst ${CURDIR}/%,%,$<)"
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c -o $@ $<

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
	@${LD} -T ${LDSCRIPT} ${OBJECTS} ${LDADD} -o ${OBJDIR}/${APP}.elf
	@${SIZE} ${OBJDIR}/${APP}.elf

${OBJDIR}/${APP}.bin: ${OBJDIR}/${APP}.elf
	@echo "  OBJCOPY ${OBJDIR}/${APP}.bin"
	@${OBJCOPY} -O binary ${OBJDIR}/${APP}.elf ${OBJDIR}/${APP}.bin

${OBJDIR}/${APP}.srec: ${OBJDIR}/${APP}.elf
	@echo "  OBJCOPY ${OBJDIR}/${APP}.srec"
	@${OBJCOPY} -O srec ${OBJDIR}/${APP}.elf ${OBJDIR}/${APP}.srec
