import sys
import os
from parser import proc0
from flags import print_flags

def proc1(config, context_str):
	if not context_str in config:
		sys.exit(4)
	context = config[context_str]

	resobj = {}

	if not 'module' in context:
		return
	for m in context['module']:
		options = ['default']
		if m in context:
			node = context[m]
			if 'options' in node:
				options += node['options']

		if context_str == 'kernel':
			p = os.path.join(osdir, "kernel", m, "mdx.kernel")
		else:
			p = os.path.join(osdir, "lib", m, "mdx.library")
		f = open(p, "r")
		data = f.read()
		f.close()
		cfg = {}
		proc0(cfg, data)

		context1 = cfg[context_str]
		for opt in options:
			incs = []
			cflags = []
			if 'incs' in context1:
				incs += context1['incs']
			if 'cflags' in context1:
				cflags += context1['cflags']
			if not opt in context1:
				continue
			node = context1[opt]
			if 'incs' in node:
				incs += node['incs']
			if 'cflags' in node:
				cflags += node['cflags']
			if not 'objects' in node:
				continue
			objects = node['objects']
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
