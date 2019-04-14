import sys
import os

args = sys.argv
if len(args) <= 1:
	sys.exit(1)

filename = args[1]
options = args[2:]

if not os.path.isfile(filename):
	sys.exit(2)

result = []

f = open(filename, "r")
for line in f:
	if line.startswith("#"):
		continue
	spl = line.strip().split()
	if not spl:
		continue
	obj = spl[0]
	dep_str = " ".join(spl[1:])
	if not dep_str:
		result.append(obj)
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
			result.append(obj)

f.close()

print(" ".join(set(result)))
