import sys
import os
from files import process

#
# Process the value
#
def proc(d, key, val):
	k = key.strip()
	v = val.strip()

	#print('proc: key "' + k + '" val "' + v + '"')

	if v.startswith('{'):
		f = {}
		proc0(f, v.strip("{}"))
		if k in d:
			d[k].append(f)
		else:
			d[k] = [f]
	elif v.startswith("'"):
		if k in d:
			d[k].append(v.strip("'"))
		else:
			d[k] = [v.strip("'")]
	else:
		spl = v.split()
		for s in spl:
			if k in d:
				d[k].append(s)
			else:
				d[k] = [s]

#
# Find the key value pair
# First word is key, everything else until ';' is value
#
def proc0(d, data):
	i = 0
	tmp = ''
	key = ''
	val = ''
	depth = 0

	#print("proc0 %s\n" % data)

	while i < len(data):
		c = data[i]

		if ((c == ';' or i == len(data) - 1) and depth == 0):
			if val == '':
				val = tmp
				tmp = ''
			if (key and val):
				proc(d, key, val)
				key = ''
				val = ''
			i += 1
			continue

		elif (c == '='):
			i += 1
			continue

		elif (c == ' ' or c == '\t'):
			if key == '' and tmp.strip():
				key = tmp
				tmp = ''

		elif (c == '{'):
			depth += 1
		elif (c == '}'):
			depth -= 1

		tmp += c
		i += 1

def proc_directive(l, d):
	if type(d) != dict:
		sys.exit(5)

	# Process directives only
	if 'options' in d:
		options = d['options']
		for opt in options:
			l.append(opt)

if __name__ == '__main__':
	args = sys.argv
	if len(args) < 2:
		sys.exit(1)

	osdir = args[1]
	config_str = args[2]

	config = {}
	proc0(config, config_str)

	if not 'kernel' in config:
		sys.exit(2)

	result = []

	# Find all the 'module' directives
	modules = []
	kernel = config['kernel']
	for k in kernel:
		if 'module' in k:
			modules += k['module']

	# For each module look for directives
	for m in modules:
		l = []
		for k in kernel:
			if m in k:
				d = k[m]
				for itm in d:
					proc_directive(l, itm)
		result += process(osdir, "kernel/%s" % m, l)

	print(" ".join(result))
