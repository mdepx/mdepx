__objdir:
	@if test -n "${MAKEOBJDIRPREFIX}"; then				\
	    if ! test -d "${MAKEOBJDIRPREFIX}/${.CURDIR}"; then		\
		mkdir -p ${MAKEOBJDIRPREFIX}/${.CURDIR};		\
		echo "Obj directory created. Run me again";		\
		exit 1;							\
	    fi;								\
	else if ! test -d "${.CURDIR}/obj"; then			\
		mkdir -p ${.CURDIR}/obj;				\
		echo "Obj directory created. Run me again";		\
		exit 1;							\
	    fi;								\
	fi
