import sys
import os
from kernel import proc0

if __name__ == '__main__':
	args = sys.argv
	if len(args) < 3:
		sys.exit(1)

	osdir = args[1]
	cmd = args[2]

	if cmd == 'libs':
		config_str = args[3]
	elif cmd == 'options':
		lib = args[3]
		config_str = args[4]
	elif cmd == 'objs' or cmd == 'incs':
		lib = args[3]
		module = args[4]
		config_str = args[5]
	else:
		sys.exit(2)

	config = {}
	proc0(config, config_str)

	if not 'library' in config:
		sys.exit(3)

	library = config['library']

	result = []

	if cmd == 'libs':
		for l in library:
			if not 'module' in l:
				continue
			for module in l['module']:
				result.append(module)


	if cmd == 'options':
		result.append('default')
		for l in library:
			if not lib in l:
				continue
			node = l[lib]
			for n in node:
				if not 'options' in n:
					continue
				options = n['options']
				for opt in options:
					result.append(opt)

	if cmd == 'objs':
		p = os.path.join(osdir, "lib", lib, "mdx.library")
		f1 = open(p, "r")
		data = f1.read()
		f1.close()
		cfg = {}
		proc0(cfg, data)
		library = cfg['library']
		for l in library:
			if not module in l:
				continue
			node = l[module]
			for n in node:
				if not 'objects' in n:
					continue
				objects = n['objects']
				for obj in objects:
					result.append(obj)

	if cmd == 'incs':
		p = os.path.join(osdir, "lib", lib, "mdx.library")
		f = open(p, "r")
		data = f.read()
		f.close()
		cfg = {}
		proc0(cfg, data)
		library = cfg['library']
		for l in library:
			if 'incs' in l:
				result += l['incs']
			if not module in l:
				continue
			m = l[module]
			for node in m:
				if 'incs' in node:
					result += node['incs']

	s = " ".join(result)
	print(s)
