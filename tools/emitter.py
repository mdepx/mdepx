#-
# Copyright (c) 2019-2020 Ruslan Bukin <br@bsdpad.com>
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

from directives import proc_directives
from builder import build
from parser import to_json, merge
from debug import warn, err
import argparse
import sys
import os

def warn(msg):
	print("Warning: %s" % msg);

def open_modules(root, context, debug):
	# Include contexts
	ky = context.copy()
	for k in ky:
		v = ky[k]
		if k in ['modules',]:
			for el in list(set(v)):
				#print("opening %s %s, root %s" % (k, el, root))
				if el in context and 'root' in context[el]:
					p = context[el]['root'][0]
				else:
					p = el
				p = os.path.join(root, p)
				p = os.path.expanduser(p)
				if not os.path.exists(p) and debug:
					warn("Directory not found: %s" % p)
				p1 = os.path.join(p, 'module.conf')
				if not os.path.exists(p1):
					continue
				with open(p1) as f:
					data = f.read()
					if not el in context:
						context[el] = {}
					to_json(context[el], data)

	# Now open modules of contexts
	ky = context.copy()
	for k in ky:
		v = ky[k]
		if type(v) == dict:
			if 'root' in v:
				p = v['root'][0]
			else:
				p = k
			p = os.path.join(root, p)
			open_modules(p, v, debug)

def handle_options(root, context):
	ky = context.copy()
	for k in ky:
		v = ky[k]
		if type(v) == dict:
			p = os.path.join(root, k)
			handle_options(p, v)

		if k == 'options':
			for opt in v:
				if opt in ky:
					if not type(ky[opt]) == dict:
						continue
					merge(context, ky[opt])

if __name__ == '__main__':

	parser = argparse.ArgumentParser()
	parser.add_argument("-d", help="Enable debugging", action="store_true")
	parser.add_argument("-j", help="Parallel build", action="store_true")
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
	to_json(config, data)

	# Merge-in module's configuration files into the main config
	root = ''
	open_modules(root, config, args.d)

	# Process options directive
	handle_options(root, config)

	result = proc_directives('', config)

	# Compile and link the app
	if not build(result, debug=args.d, parallel=args.j):
		sys.exit(3)
