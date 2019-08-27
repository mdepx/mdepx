import sys
import os

def proc(result, m, d):
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

		# Process everything else
		d1 = d[k]
		if type(d1) == list:
			for itm in d1:
				if type(itm) == str:
					s = "%s_%s" % (m, k)
					result[s] = itm
				elif type(itm) == dict:
					proc(result, "%s_%s" % (m, k), itm)
				else:
					sys.exit(5)
		elif type(d1) == dict:
			proc(result, "%s_%s" % (m, k), d1)

def print_flags(config):
	kernel = config['kernel']
	if type(kernel) != dict:
		sys.exit(4)

	modules = []
	if 'module' in kernel:
		modules += kernel['module']

	result = {}
	for m in modules:
		result[m] = ''
		if not m in kernel:
			continue
		d = kernel[m]
		proc(result, m, d)

	for key in result:
		val = result[key]
		if val:
			print("CFLAGS+=-DMDX_%s=%s" % \
				(key.upper(), val.upper()))
		else:
			print("CFLAGS+=-DMDX_%s" % key.upper())
