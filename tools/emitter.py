import sys
import os
from parser import proc0
from flags import collect_all_user_flags
from flags import collect_flags, print_flags

c_dirs = {'library': 'lib', 'kernel': 'kernel'}

def obj_set_flags(resobj, context, key, l):
	if not 'objects' in context:
		return
	for obj in context['objects']:
		if not obj in resobj:
			resobj[obj] = {}
		if not key in resobj[obj]:
			resobj[obj][key] = []
		for el in l:
			resobj[obj][key].append(el)

def proc1(resobj, flags, config, context_str):
	if not context_str in config:
		return resobj
	context = config[context_str]
	if not 'module' in context:
		return
	for m in context['module']:
		options = []
		if m in context:
			node = context[m]
			if 'options' in node:
				options += node['options']

		p = os.path.join(osdir, c_dirs[context_str], m, "mdepx.conf")
		f = open(p, "r")
		data = f.read()
		f.close()
		cfg = {}
		proc0(cfg, data)

		if not m in cfg:
			continue

		context1 = cfg[m]
		collect_flags(flags, m, context1, False)

		incs = []
		cflags = []

		if 'incs' in context1:
			incs += context1['incs']
		if 'cflags' in context1:
			cflags += context1['cflags']

		obj_set_flags(resobj, context1, 'incs', incs)
		obj_set_flags(resobj, context1, 'cflags', cflags)

		for opt in options:
			incs1 = []
			cflags1 = []

			if not opt in context1:
				continue
			node = context1[opt]
			collect_flags(flags, "%s_%s" % (m, opt), node, False)

			if 'incs' in node:
				incs1 += node['incs']
			if 'cflags' in node:
				cflags1 += node['cflags']

			obj_set_flags(resobj, node, 'incs', incs1 + incs)
			obj_set_flags(resobj, node, 'cflags', cflags1 + cflags)

	return resobj

def emit_objects_flags(resobj):
	for obj in resobj:
		print("OBJECTS+=%s/%s" % (osdir, obj))

	for obj in resobj:
		if 'incs' in resobj[obj]:
			for inc in resobj[obj]['incs']:
				print("CFLAGS_%s/%s+=-I${OSDIR}/%s" % \
					(osobjdir, obj, inc))

		if 'cflags' in resobj[obj]:
			for cflag in resobj[obj]['cflags']:
				print("CFLAGS_%s/%s+=%s" % \
					(osobjdir, obj, cflag))

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
