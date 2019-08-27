import sys
import os

def process_node(result, m, d):
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
		for itm in d1:
			if type(itm) == str:
				s = "%s_%s" % (m, k)
				result[s] = itm
			elif type(itm) == dict:
				process_node(result, "%s_%s" % (m, k), itm)
			else:
				sys.exit(5)

def print_flags(config):
	kernel = config['kernel']
	if type(kernel) != list:
		sys.exit(4)

	modules = []
	for k in kernel:
		if 'module' in k:
			modules += k['module']

	result = {}
	for m in modules:
		result[m] = ''
		for k in kernel:
			if not m in k:
				continue
			d = k[m]
			for el in d:
				process_node(result, m, el)

	for key in result:
		val = result[key]
		if val:
			print("CFLAGS+=-DMDX_%s=%s" % \
			    (key.upper(), val.upper()))
		else:
			print("CFLAGS+=-DMDX_%s" % key.upper())
