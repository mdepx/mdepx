.ifndef APP
.error "APP must be defined"
.endif

.if exists(${HOME}/.osfive/${APP}.mk)
.include "${HOME}/.osfive/${APP}.mk"
.endif
