import sys
import os

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
