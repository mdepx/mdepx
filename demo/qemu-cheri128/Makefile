APP = qemu-cheri128

OSDIR = ../../
OBJDIR = obj

BASE_ADDR =	0xffffffff80100000

LDSCRIPT_TPL =	${CURDIR}/ldscript.tpl
LDSCRIPT =	${OBJDIR}/ldscript

CMD = python3 -B ${OSDIR}/tools/emitter.py

hybrid:	${LDSCRIPT}
	@${CMD} mdepx.conf

pure:	${LDSCRIPT}
	@${CMD} mdepx-pure.conf

${LDSCRIPT}:
	@sed s#__BASE_ADDR__#${BASE_ADDR}#g ${LDSCRIPT_TPL} > ${LDSCRIPT}

clean:
	@rm -rf obj/*

include ${OSDIR}/mk/user.mk
