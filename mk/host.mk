_HOST_OSNAME != uname -s
_HOST_OSREL != uname -r
_HOST_OSVER = ${_HOST_OSREL:C/-.*//}
_HOST_HOSTNAME != hostname
