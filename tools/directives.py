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
import copy
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

def process_directives(root, context, data, r):
	for x in data:
		if x in ['prefix']:
			l = root.replace(data[x][0], data[x][1][0])
			l = l.replace("/", "_")
			#print(l)
			flags = r['flags']
			flags[l] = ''
			collect_flags(flags, l, context, False)

	# vars dictionary is for global directives,
	# which are 'link' and 'machine' only.
	vars = r['vars']

	for x in context:
		args = context[x]
		if x in ['link']:
			if (len(args) % 2) != 0:
				print("Error: link directive accepts "
					"even number of arguments")
				sys.exit(5)
			vars[x] = args
		elif x in ['machine']:
			vars[x] = os.path.join(root, args[0])
		elif x in ['objects']:
			for obj in args:
				o = os.path.join(root, obj)
				resobj = r['resobj']
				if not o in resobj:
					resobj[o] = {}

				# Now we apply all the nested directives
				# to this particular object
				for key in data:
					resobj[o][key] = []
					for el in data[key]:
						resobj[o][key].append(el)
		elif x in ['module']:
			for m in args:
				#print('process_dmodule %s' % m)
				if m in context:
					node = context[m]
					if 'root' in node:
						p = os.path.join(root,
							node['root'][0])
					else:
						p = os.path.join(root, m)
					find_directives(p, node, data, r)

def find_directives(root, context, data, result):
	#print("directives '%s'" % context)
	data1 = copy.deepcopy(data)
	collect_nested_directives(root, context, data1)
	process_directives(root, context, data1, result)

def proc_directives(root, context):

	tmp = {}
	result = {'resobj': {},
		  'flags': {},
		  'vars': {}}
	find_directives(root, context, tmp, result)

	return result
