ifndef APP
$(error Error: APP must be defined)
endif

ifneq ("$(wildcard ${HOME}/.mdepx/${APP}.mk)", "")
include ${HOME}/.mdepx/${APP}.mk
endif
