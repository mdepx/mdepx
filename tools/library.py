import sys
import os
from kernel import proc0

if __name__ == '__main__':
	args = sys.argv
	if len(args) < 3:
		sys.exit(1)

	config_file = args[1]
	objdir = args[2]
	osdir = args[3]
	cmd = args[4]

	osobjdir = "%s/%s" % (objdir, osdir)
	if cmd != 'library':
		sys.exit(2)

	if not os.path.exists(config_file):
		sys.exit(3)

	f = open(config_file, "r")
	config_str = f.read()
	f.close()

	config = {}
	proc0(config, config_str)

	if not 'library' in config:
		sys.exit(4)

	library = config['library']

	result = []
	modules = {}

	for l in library:
		if not 'module' in l:
			continue
		for module in l['module']:
			modules[module] = ['default']

	for m in modules:
		for l in library:
			if not m in l:
				continue
			node = l[m]
			for n in node:
				if not 'options' in n:
					continue
				options = n['options']
				for opt in options:
					modules[m].append(opt)

	#print(modules)

	resobj = {}
	for m in modules:
		p = os.path.join(osdir, "lib", m, "mdx.library")
		f = open(p, "r")
		data = f.read()
		f.close()
		cfg = {}
		proc0(cfg, data)
		library = cfg['library']
		for l in library:
			for opt in modules[m]:
				result = []
				if 'incs' in l:
					result += l['incs']
				if not opt in l:
					continue
				node = l[opt]
				for n in node:
					if 'incs' in n:
						result += n['incs']
					if not 'objects' in n:
						continue
					objects = n['objects']
					for obj in objects:
						resobj[obj] = []
						for inc in result:
							resobj[obj].append(inc)

	for obj in resobj:
		print("OBJECTS+=%s/%s" % (osdir, obj))

	for obj in resobj:
		for inc in resobj[obj]:
			print("CFLAGS_%s/%s+=-I${OSDIR}/%s" % \
				(osobjdir, obj, inc))
