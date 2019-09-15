import sys
import os

def merge(a, b, path = None):
	"merges b into a"
	if path is None: path = []
	for key in b:
		if key in a:
			if type(a[key]) == dict and type(b[key]) == dict:
				merge(a[key], b[key], path + [str(key)])
			elif a[key] == b[key]:
				pass # same leaf value
			else:
				if key.endswith("+"):
					a[key] += b[key]
				else:
					# conflict, keep original value
					pass
		else:
			a[key] = b[key]
	return a

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
			merge(d[k], f)
			#for k1 in f:
			#	d[k][k1] = f[k1]
		else:
			d[k] = f
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

		if (c == '#' and depth == 0):
			# Single line comment
			while c != '\n':
				c = data[i]
				i += 1
			continue

		elif ((c == ';' or i == len(data) - 1) and depth == 0):
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
			if tmp.strip() == '':
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
