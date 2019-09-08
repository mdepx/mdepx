from flags import collect_flags
from flags import print_flags
from parser import proc0
import copy
import sys
import os

def collect_nested_directives(root, context, data):

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

def process_directives(root, context, data):

	if 'objects' in context:
		for obj in context['objects']:
			o = os.path.join(root, obj)
			if not o in resobj:
				resobj[o] = {}
			for key in data:
				resobj[o][key] = []
				for el in data[key]:
					resobj[o][key].append(el)

	if 'prefix' in data:
		l = root.replace(data['prefix'][0], data['prefix'][1][0])
		l = l.replace("/", "_")
		flags[l] = ''
		collect_flags(flags, l, context, False)

	if 'module' in context:
		for m in context['module']:
			if m in context:
				node = context[m]
				p = os.path.join(root, m)
				proc1(p, node, data)

	if 'options' in context:
		for opt in context['options']:
			if opt in context:
				node = context[opt]
				proc1(root, node, data)

def proc1(root, context, data):
	data1 = copy.deepcopy(data)
	collect_nested_directives(root, context, data1)
	process_directives(root, context, data1)

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
	proc1('', config, data)

	#print(config)
	emit_objects_flags(resobj)

	print_flags(flags)
