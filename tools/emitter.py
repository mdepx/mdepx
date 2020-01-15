#-
# Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#

from flags import collect_flags
from builder import build
from parser import proc0
import argparse
import copy
import sys
import os

def collect_nested_directives(root, context, data):

	# Replace root if provided in the context
	if 'root' in context:
		root = context['root'][0]

	for x in context:

		spl = x.split("-")
		if len(spl) < 2:
			continue

		cmd = spl[0]
		if not cmd in ['set', 'append', 'prepend']:
			continue

		k = "-".join(spl[1:])
		if not k in ['cflags', 'aflags',
				'build-flags', 'search-path']:
			continue

		if not k in data or cmd == 'set':
			data[k] = []

		for el in context[x]:
			if k in ['search-path']:
				v = os.path.join(root, el)
			else:
				v = el

			if cmd == 'prepend':
				data[k].insert(0, v)
			else:
				data[k].append(v)

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
		elif x in ['archive']:
			if (len(args) > 1):
				print("Error: archive directive accepts "
					"one argument only (filename).")
				sys.exit(5)
			vars[x] = args
		elif x in ['machine']:
			vars[x] = os.path.join(root, args[0])
		elif x in ['ldadd']:
			if not x in vars:
				vars[x] = []
			for l in args:
				vars[x].append(os.path.join(root, l))
		elif x in ['objects']:
			for obj in args:
				o = os.path.join(root, obj)
				if not o in resobj:
					resobj[o] = {}
				for key in data:
					resobj[o][key] = []
					for el in data[key]:
						resobj[o][key].append(el)
		elif x in ['options']:
			for opt in args:
				if opt in context:
					node = context[opt]
					proc1(root, node, data)
		elif x in ['module']:
			for m in args:
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
