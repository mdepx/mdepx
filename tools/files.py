import sys
import os
from flags import normalize

def process(osdir, dir, opts):
	result = []
	filename = os.path.join(dir, "files")
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
		obj = os.path.join(dir, spl[0])
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
					if d in opts:
						add = 0
						break
				elif dep not in opts:
					add = 0
					break
			if add:
				result.append(obj)
	f.close()
	return result

if __name__ == '__main__':
	args = sys.argv
	if len(args) < 2:
		sys.exit(1)

	osdir = args[1]
	options = normalize(args[2])
	result = []
	for dir in options.keys():
		kdir = os.path.join("kernel", dir);
		result += process(osdir, kdir, options[dir])

	print(" ".join(set(result)))
