import sys
import os
from parser import proc0
from flags import print_flags

def proc1(config, context_str):
	if not context_str in config:
		sys.exit(4)

	context = config[context_str]

	result = []
	modules = {}

	for l in context:
		if not 'module' in l:
			continue
		for module in l['module']:
			modules[module] = ['default']

	for m in modules:
		for l in context:
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
		if context_str == 'kernel':
			p = os.path.join(osdir, "kernel", m, "mdx.kernel")
		else:
			p = os.path.join(osdir, "lib", m, "mdx.library")
		f = open(p, "r")
		data = f.read()
		f.close()
		cfg = {}
		proc0(cfg, data)
		context = cfg[context_str]
		for l in context:
			for opt in modules[m]:
				incs = []
				cflags = []
				if 'incs' in l:
					incs += l['incs']
				if 'cflags' in l:
					cflags += l['cflags']
				if not opt in l:
					continue
				node = l[opt]
				for n in node:
					if 'incs' in n:
						incs += n['incs']
					if 'cflags' in n:
						cflags += n['cflags']
					if not 'objects' in n:
						continue
					objects = n['objects']
					for obj in objects:
						resobj[obj] = {'incs': [],
								'cflags': []}
						for inc in incs:
							resobj[obj]['incs'].append(inc)
						for cflag in cflags:
							resobj[obj]['cflags'].append(cflag)

	for obj in resobj:
		print("OBJECTS+=%s/%s" % (osdir, obj))

	for obj in resobj:
		for inc in resobj[obj]['incs']:
			print("CFLAGS_%s/%s+=-I${OSDIR}/%s" % \
				(osobjdir, obj, inc))
		for cflag in resobj[obj]['cflags']:
			print("CFLAGS_%s/%s+=%s" % (osobjdir, obj, cflag))

if __name__ == '__main__':
	args = sys.argv
	if len(args) < 4:
		sys.exit(1)

	config_file = args[1]
	objdir = args[2]
	osdir = args[3]
	osobjdir = "%s/%s" % (objdir, osdir)

	if not os.path.exists(config_file):
		sys.exit(2)
	f = open(config_file, "r")
	config_str = f.read()
	f.close()

	config = {}
	proc0(config, config_str)
	proc1(config, 'library')
	proc1(config, 'kernel')
	print_flags(config)
