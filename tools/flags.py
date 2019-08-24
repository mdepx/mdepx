import sys
import os
from kernel import proc0

def wr(s):
	sys.stdout.write(s.upper())

def proc2(m, d):
	for k in d:
		# Process directives
		if k == 'include':
			options = d['include']
			for opt in options:
				wr("%s_%s " % (m, opt))
			continue

		# Process everything else
		d1 = d[k]
		for itm in d1:
			if type(itm) == str:
				wr("%s_%s=%s " % (m, k, itm))
			elif type(itm) == dict:
				proc2("%s_%s" % (m, k), itm)
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

	for m in modules:
		wr("%s " % m)
		for k in kernel:
			if not m in k:
				continue
			d = k[m]
			for el in d:
				proc2(m, el)
