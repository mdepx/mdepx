from flags import collect_flags
from builder import build
from parser import proc0
import argparse
import copy
import sys
import os

def collect_nested_directives(root, context, data):

	# Replace root if provided in the context
	for x in context:
		if x in ['root']:
			root = context[x][0]

	if 'set-build-flags' in context:
		if not 'build-flags' in data:
			data['build-flags'] = []
		data['build-flags'] += context['set-build-flags']

	if 'append-cflags' in context:
		if not 'cflags' in data:
			data['cflags'] = []
		data['cflags'] += context['append-cflags']

	if 'append-asflags' in context:
		if not 'asflags' in data:
			data['asflags'] = []
		data['asflags'] += context['append-asflags']

	if 'set-cflags' in context:
		data['cflags'] = context['set-cflags']
	if 'set-asflags' in context:
		data['asflags'] = context['set-asflags']

	for x in ['append-search-path']:
		if x in context:
			if not 'search-path' in data:
				data['search-path'] = []
			for el in context[x]:
				data['search-path'].append(os.path.join(root,
									el))

	for x in ['set-search-path']:
		if x in context:
			data['search-path'] = []
			for el in context[x]:
				data['search-path'].append(os.path.join(root,
									el))

	for x in ['objdir']:
		if x in context:
			data[x] = context[x]

	if 'prefix' in context:
		data['prefix'] = [root, context['prefix']]

def process_directives(root, context, data):

	for x in data:
		if x in ['prefix']:
			l = root.replace(data[x][0], data[x][1][0])
			l = l.replace("/", "_")
			#print(l)
			flags[l] = ''
			collect_flags(flags, l, context, False)

	for x in context:
		args = context[x]
		if x in ['link']:
			if (len(args) % 2) != 0:
				print("Error: link directive accepts "
					"even number of arguments")
				sys.exit(5)
			vars[x] = args
		elif x in ['machine']:
			vars[x] = os.path.join(root, context[x][0])
		elif x in ['ldadd']:
			if not x in vars:
				vars[x] = []
			for l in context[x]:
				vars[x].append(os.path.join(root, l))
		elif x in ['objects']:
			for obj in context[x]:
				o = os.path.join(root, obj)
				if not o in resobj:
					resobj[o] = {}
				for key in data:
					resobj[o][key] = []
					for el in data[key]:
						resobj[o][key].append(el)
		elif x in ['options']:
			for opt in context[x]:
				if opt in context:
					node = context[opt]
					proc1(root, node, data)
		elif x in ['module']:
			for m in context[x]:
				if m in context:
					node = context[m]
					if 'root' in node:
						p = os.path.join(root,
							node['root'][0])
					else:
						p = os.path.join(root, m)
					proc1(p, node, data)

def proc1(root, context, data):
	data1 = copy.deepcopy(data)
	collect_nested_directives(root, context, data1)
	process_directives(root, context, data1)

	return resobj

def open_modules(root, context):

	ky = context.copy()

	for k in ky:
		v = context[k]
		if k == 'module':
			for el in v:
				if el in context and 'root' in context[el]:
					p = context[el]['root'][0]
				else:
					p = os.path.join(root, el)
				p = os.path.expanduser(p)
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
			if 'root' in v:
				p = v['root'][0]
			else:
				p = os.path.join(root, k)
			open_modules(p, v)

if __name__ == '__main__':

	parser = argparse.ArgumentParser()
	parser.add_argument("-d", help="enable debugging", action="store_true")
	parser.add_argument("conf", help="A working config file")
	args = parser.parse_args()

	config_file = args.conf

	# Open main configuration file provided by app
	if not os.path.exists(config_file):
		print("Error: config file not found: %s" % config_file)
		sys.exit(2)

	with open(config_file) as f:
		data = f.read()

	config = {}
	proc0(config, data)

	# Merge-in module's configuration files into the main config
	open_modules('', config)

	resobj = {}
	flags = {}
	vars = {}

	proc1('', config, {})

	# Compile and link the app
	if not build(resobj, flags, vars, debug=args.d):
		sys.exit(3)
