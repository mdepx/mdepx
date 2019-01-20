ifndef APP
error "APP must be defined"
endif

ifneq ("$(wildcard ${HOME}/.osfive/${APP}.mk)","")
include ${HOME}/.osfive/${APP}.mk
endif
