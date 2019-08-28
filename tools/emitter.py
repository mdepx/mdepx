import sys
import os
from parser import proc0
from flags import collect_all_user_flags
from flags import collect_flags, print_flags

def proc1(resobj, flags, config, context_str):
	if not context_str in config:
		sys.exit(4)
	context = config[context_str]
	if not 'module' in context:
		return
	for m in context['module']:
		options = []
		if m in context:
			node = context[m]
			if 'options' in node:
				options += node['options']

		if context_str == 'kernel':
			p = os.path.join(osdir, "kernel", m, "config.mdx")
		else:
			p = os.path.join(osdir, "lib", m, "config.mdx")
		f = open(p, "r")
		data = f.read()
		f.close()
		cfg = {}
		proc0(cfg, data)

		context1 = cfg[m]
		collect_flags(flags, m, context1, False)

		incs = []
		cflags = []

		if 'incs' in context1:
			incs += context1['incs']
		if 'cflags' in context1:
			cflags += context1['cflags']

		if 'objects' in context1:
			objects = context1['objects']
			for obj in objects:
				resobj[obj] = {'incs': [], 'cflags': []}
				for inc in incs:
					resobj[obj]['incs'].append(inc)
				for cflag in cflags:
					resobj[obj]['cflags'].append(cflag)

		for opt in options:

			if not opt in context1:
				continue
			node = context1[opt]
			collect_flags(flags, "%s_%s" % (m, opt), node, False)

			if 'incs' in node:
				incs += node['incs']
			if 'cflags' in node:
				cflags += node['cflags']

			if not 'objects' in node:
				continue
			objects = node['objects']
			for obj in objects:
				resobj[obj] = {'incs': [], 'cflags': []}
				for inc in incs:
					resobj[obj]['incs'].append(inc)
				for cflag in cflags:
					resobj[obj]['cflags'].append(cflag)
	return resobj

def emit_objects_flags(resobj):
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

	flags = {}

	resobj = {}
	proc1(resobj, flags, config, 'library')
	proc1(resobj, flags, config, 'kernel')
	emit_objects_flags(resobj)

	collect_all_user_flags(flags, config['kernel'])

	print_flags(flags)
