from flags import collect_flags
from flags import print_flags
from parser import proc0
import copy
import sys
import os

def collect_directives(root, context, data):

	for x in ['incs', 'incs+', 'cflags', 'cflags+']:
		if x in context:
			if x.endswith("+"):
				k = x.rstrip("+")
				if not k in data:
					data[k] = []
				data[k] += context[x]
			else:
				data[x] = context[x]

	if 'prefix' in context:
		data['prefix'] = [root, context['prefix']]

	if 'module' in context:
		pass

def process_directives(resobj, root, m, context, data):

	if 'objects' in context:
		p = os.path.join(root, m)
		for obj in context['objects']:
			o = os.path.join(p, obj)
			if not o in resobj:
				resobj[o] = {}
			for key in data:
				resobj[o][key] = []
				for el in data[key]:
					resobj[o][key].append(el)

	if 'prefix' in data:
		l = root.replace(data['prefix'][0], data['prefix'][1][0])
		l = l.replace("/", "_")
		if m:
			l += "_%s" % m
		flags[l] = ''
		collect_flags(flags, l, context, False)

def proc1(resobj, flags, root, context, data):
	data1 = copy.deepcopy(data)
	collect_directives(root, context, data1)
	process_directives(resobj, root, '', context, data1)

	if not 'module' in context:
		return
	for m in context['module']:
		options = []
		if m in context:
			node = context[m]
			if 'options' in node:
				options += node['options']
			p = os.path.join(root, m)
			proc1(resobj, flags, p, node, data1)

		data2 = copy.deepcopy(data1)
		if m in context:
			collect_directives(p, context[m], data2)
			process_directives(resobj, root, m, context[m], data2)

		for opt in options:
			if not opt in context[m]:
				continue
			node = context[m][opt]

			p = os.path.join(root, m)
			data3 = copy.deepcopy(data2)
			collect_directives(p, node, data3)
			process_directives(resobj, root, m, node, data3)

	return resobj

def emit_objects_flags(resobj):
	for obj in resobj:
		print("OBJECTS+=%s" % obj)

	for obj in resobj:
		if 'incs' in resobj[obj]:
			for inc in resobj[obj]['incs']:
				print("CFLAGS_%s/%s+=-I${OSDIR}/%s" % \
					(objdir, obj, inc))

		if 'cflags' in resobj[obj]:
			for cflag in resobj[obj]['cflags']:
				print("CFLAGS_%s/%s+=%s" % \
					(objdir, obj, cflag))

def open_modules(root, context):
	ky = context.copy()
	for k in ky:
		v = context[k]
		if k == 'module':
			for el in v:
				p = os.path.join(root, el)
				p1 = os.path.join(p, 'mdepx.conf')
				if not os.path.exists(p1):
					continue
				with open(p1) as f:
					data = f.read()
					if not el in context:
						context[el] = {}
					proc0(context, data)
					open_modules(p, context[el])
		elif type(v) == dict:
			p = os.path.join(root, k)
			open_modules(p, v)

if __name__ == '__main__':
	args = sys.argv
	if len(args) < 3:
		sys.exit(1)

	config_file = args[1]
	objdir = args[2]

	# Open main configuration file provided by app
	if not os.path.exists(config_file):
		sys.exit(2)
	with open(config_file) as f:
		data = f.read()

	config = {}
	proc0(config, data)

	# Merge-in module's configuration files into the main config
	open_modules('', config)

	resobj = {}
	flags = {}
	data = {}
	proc1(resobj, flags, '', config, data)

	#print(config)
	emit_objects_flags(resobj)

	print_flags(flags)
