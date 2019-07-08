import sys
import os
from flags import normalize

def process(osdir, filename):
	result = []
	fullpath = os.path.join(osdir, filename)
	if not os.path.isfile(fullpath):
		sys.exit(2)
	f = open(fullpath, "r")
	for line in f:
		if line.startswith("include"):
			spl = line.split()
			include = spl[1].strip()
			result += process(osdir, include)
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
				if dep.startswith("!"):
					d = dep.lstrip("!")
					if d in options:
						add = 0
						break
				elif dep not in options:
					add = 0
					break
			if add:
				result.append(obj)
	f.close()
	return result

if __name__ == '__main__':
	args = sys.argv
	if len(args) < 3:
		sys.exit(1)

	osdir = args[1]
	filename = args[2]
	options = normalize(args[3])

	result = process(osdir, filename)
	print(" ".join(set(result)))
