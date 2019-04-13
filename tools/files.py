import sys

args = sys.argv
if len(args) <= 1:
	sys.exit(1)

f = open(args[1], "r")
options = args[2:]

result = []

for line in f:
	spl = line.strip().split()
	filename = spl[0]
	if filename.startswith("#"):
		continue
	dep_str = " ".join(spl[1:])
	if not dep_str:
		result.append(filename)
		continue
	groups = dep_str.split("|")
	for group in groups:
		deps = group.split()
		add = 1
		for dep in deps:
			if dep not in options:
				add = 0
				break
		if add:
			result.append(filename)

if result:
	print(" ".join(set(result)))
