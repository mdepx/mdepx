import sys
import os
from kernel import proc0

def proc2(result, m, d):
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
				proc2(result, "%s_%s" % (m, k), itm)
			else:
				sys.exit(5)

if __name__ == '__main__':
	args = sys.argv
	if len(args) < 1:
		sys.exit(1)
	config_str = args[1]
	config = {}
	proc0(config, config_str)
	if not 'kernel' in config:
		sys.exit(2)
	kernel = config['kernel']

	if type(kernel) != list:
		sys.exit(3)

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
				proc2(result, m, el)

	for key in result:
		val = result[key]
		if val:
			print("%s=%s" % (key.upper(), val.upper()))
		else:
			print("%s" % key.upper())
