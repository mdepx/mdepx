import sys
import os

def prs(str):
	d = {}

	tmp = ''
	opt = ''
	i = 0
	while i < len(str):
		c = str[i]

		if (c == ' ' or c == '['):
			if tmp:
				opt = tmp
				d[opt] = ''
				tmp = ''
		else:
			tmp += c
			if (i == len(str) - 1):
				d[tmp] = ''

		if (c == '['):
			# find the end
			j = i
			while j < len(str):
				c1 = str[j]
				if (c1 == ']'):
					if opt:
						d[opt] = str[i + 1:j]
					i = j
					break
				j += 1
		i += 1

	return d

def normalize(options):
	d = {}

	tmp = ''
	opt = ''
	i = 0
	while i < len(options):
		c = options[i]

		if (c == ' ' or c == '('):
			if tmp:
				opt = tmp
				d[opt] = {}
				tmp = ''
		else:
			tmp += c
			if (i == len(options) - 1):
				d[tmp] = {}

		if (c == "("):
			# find the end
			j = i
			while j < len(options):
				c1 = options[j]
				if (c1 == ")"):
					if opt:
						op = options[i + 1:j]
						d[opt] = prs(op.strip())
					i = j
					break
				j += 1
		i += 1

	return d

if __name__ == '__main__':
	args = sys.argv
	if len(args) < 1:
		sys.exit(1)
	options = args[1]

	dres = normalize(options)

	res = []
	for key in dres.keys():
		res.append(key)
		d = dres[key]
		#print(key, type(dres[key]), dres[key])
		for o in d.keys():
			res.append("%s_%s" % (key, o))
			spl = d[o].split()
			for s in spl:
				res.append("%s_%s_%s" % (key, o, s))

	res = ' '.join(res)
	print(res.upper())
