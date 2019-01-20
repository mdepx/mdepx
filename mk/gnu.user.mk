ifndef APP
error "APP must be defined"
endif

ifneq ("$(wildcard ${HOME}/.osfive/${APP}.gnu.mk)","")
include ${HOME}/.osfive/${APP}.gnu.mk
endif
