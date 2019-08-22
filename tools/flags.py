import sys
import os
from kernel import proc0

def wr(s):
	sys.stdout.write(s.upper())

def proc1(m, d):
	for k in d:
		# Process directives
		if k == 'include':
			options = d['include']
			if type(options) == list:
				for opt in options:
					wr("%s_%s " % (m, opt))
			else:
				wr("%s_%s " % (m, options))
			continue

		# Process everything else
		d1 = d[k]
		if type(d1) == str:
			wr("%s_%s=%s " % (m, k, d1))
		elif type(d1) == dict:
			wr("%s_" % m)
			proc1(k, d1)
		elif type(d1) == list:
			# take last value
			d2 = d1[-1]
			if type(d2) == str:
				wr("%s_%s=%s " % (m, k, d2))
			elif type(d2) == dict:
				wr("%s_" % m)
				proc1(k, d2)

if __name__ == '__main__':
	args = sys.argv
	if len(args) < 1:
		sys.exit(1)
	data = args[1]

	modules = {}
	proc0(modules, data)

	for m in modules['module']:
		if not m in modules:
			continue

		d = modules[m]
		wr("%s " % m)
		proc1(m, d)
