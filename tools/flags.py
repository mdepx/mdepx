import sys
import os

args = sys.argv
if len(args) < 1:
	sys.exit(1)

options = args[1]

def parse_opt(opt, str):
	print("opt: %s, str: %s" % (opt, str))

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
	u = key.upper()
	res.append(u)
	spl = d[key].strip('() ').split()
	for o in spl:
		res.append("%s_%s" % (u, o.upper()))

print(" ".join(res))
