#-
# Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#

import sys
import os

def collect_flags(result, m, d, deep):
	if type(d) != dict:
		sys.exit(6)

	for k in d:
		# Process directives
		if k == 'options':
			options = d['options']
			for opt in options:
				s = "%s_%s" % (m, opt)
				result[s] = ''
			continue

		if k in ['objects',
			'set-search-path',
			'set-cflags',
			'append-search-path',
			'append-cflags',
			'machine',
			'ldscript',
			'prefix',
			'module']:
			continue

		# Process everything else
		d1 = d[k]
		if type(d1) == list:
			# Take first value only
			s = "%s_%s" % (m, k)
			if type(d1[0]) == str:
				result[s] = d1[0]
			else:
				sys.exit(5)
		elif type(d1) == dict and deep == True:
			collect_flags(result, "%s_%s" % (m, k), d1, True)
