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

		if k in ['objects', 'incs', 'incs+', 'cflags', 'cflags+']:
			continue

		# Process everything else
		d1 = d[k]
		if type(d1) == list:
			for itm in d1:
				if type(itm) == str:
					s = "%s_%s" % (m, k)
					result[s] = itm
				else:
					sys.exit(5)
		elif type(d1) == dict and deep == True:
			collect_flags(result, "%s_%s" % (m, k), d1, True)

def collect_all_user_flags(flags, context):
	if 'module' in context:
		for m in context['module']:
			flags[m] = ''
			if not m in context:
				continue
			d = context[m]
			collect_flags(flags, m, d, True)

def print_flags(result):
	for key in result:
		val = result[key]
		if val:
			print("CFLAGS+=-DMDX_%s=%s" % \
				(key.upper(), val.upper()))
		else:
			print("CFLAGS+=-DMDX_%s" % key.upper())
