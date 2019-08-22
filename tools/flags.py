import sys
import os

from kernel import proc0

result = []

def wr(s):
	sys.stdout.write(s.upper())

def proc1(m, d):
	inc = []
	exc = []

	# Process directives
	if 'include' in d:
		inc = d['include']

	if type(inc) == list:
		for i in inc:
			wr(m + '_' + i + ' ')
	else:
		wr(m + '_' + inc + ' ')

	# Process everything else

	for k in d:
		if k in ['include',]:
			continue

		d1 = d[k]
		#print(d1)
		if type(d1) == str:
			val = m + '_' + k + '=' + d1
			wr(val + ' ')
		elif type(d1) == dict:
			val = m + '_'
			wr(val)
			proc1(k, d1)
		elif type(d1) == list:
			# take last value
			d2 = d1[-1]
			if type(d2) == str:
				val = m + '_' + k + '=' + d2
				wr(val + ' ')
			elif type(d2) == dict:
				val = m + '_'
				wr(val)
				proc1(k, d2)

if __name__ == '__main__':
	args = sys.argv
	if len(args) < 1:
		sys.exit(1)
	data = args[1]

	modules = {}
	proc0(modules, data)

	for m in modules['module']:
		if m in modules:
			d = modules[m]
			wr(m + ' ')
			proc1(m, d)
