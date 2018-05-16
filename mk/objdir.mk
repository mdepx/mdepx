objdir:
	@if test -n "${MAKEOBJDIRPREFIX}"; then				\
	    if ! test -d "${MAKEOBJDIRPREFIX}/${.CURDIR}"; then		\
		mkdir -p ${MAKEOBJDIRPREFIX}/${.CURDIR};		\
		echo "Obj directory created. Run me again";		\
		exit 1;							\
		fi							\
	fi
