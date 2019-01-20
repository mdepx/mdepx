.ifndef APP
.error "APP must be defined"
.endif

.if exists(${HOME}/.osfive/${APP}.bsd.mk)
.include "${HOME}/.osfive/${APP}.bsd.mk"
.endif
