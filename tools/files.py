import sys
import os

args = sys.argv
if len(args) <= 1:
	sys.exit(1)

osdir = args[1]
filename = args[2]
options = args[3:]

def process(fullpath):
	if not os.path.isfile(fullpath):
		sys.exit(2)
	f = open(fullpath, "r")
	for line in f:
		if line.startswith("include"):
			spl = line.split()
			include = spl[1].strip()
			process(os.path.join(osdir, include))
			continue
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

result = []

process(os.path.join(osdir, filename))

print(" ".join(set(result)))
