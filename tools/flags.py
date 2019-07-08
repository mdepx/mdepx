import sys
import os

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
				d[opt] = ''
				tmp = ''
		else:
			tmp += c
			if (i == len(options) - 1):
				d[tmp] = ''

		if (c == "("):
			# find the end
			j = i
			while j < len(options):
				c1 = options[j]
				if (c1 == ")"):
					if opt:
						d[opt] = options[i:j + 1]
					i = j
					break
				j += 1
		i += 1

	res = []
	for key in d.keys():
		res.append(key)
		spl = d[key].strip('() ').split()
		for o in spl:
			res.append("%s_%s" % (key, o))

	return ' '.join(res)

if __name__ == '__main__':
	args = sys.argv
	if len(args) < 1:
		sys.exit(1)
	options = args[1]

	res = normalize(options)
	print(res.upper())
