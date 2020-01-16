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

import os

DEFAULT_OBJDIR = 'obj'

def machine(vars, objdir):
	machine = vars.get('machine')
	if not machine:
		print("Error: machine directive is not set")
		return False

	m = os.path.abspath(machine)
	if not os.path.exists(m):
		print("Error: machine headers not found at path: %s" % m)
		return False

	dst = os.path.join(objdir, 'machine')
	if os.path.lexists(dst):
		os.unlink(dst)
	os.symlink(m, dst)

	return True

def find_elf(name):
	name = os.path.expanduser(name)
	for path in os.environ["PATH"].split(os.pathsep):
		elf = os.path.join(path, name)
		if os.path.exists(elf):
			return elf
	return None

def run(cmd, args):
	pid = os.fork()
	if pid == 0:
		os.execve(cmd, args, os.environ)

	t = os.waitpid(pid, 0)
	return t[1]

def compile(resobj, flags, vars, link_objs, debug):

	cc = os.environ.get('CC', '')
	if cc:
		compiler = cc
	else:
		cross_compile = os.environ.get('CROSS_COMPILE', '')
		compiler = '%sgcc' % cross_compile

	compiler_fp = find_elf(compiler)
	if not compiler_fp:
		print("compiler not found in PATH: %s" % compiler)
		return False

	env_cflags = os.environ.get('CFLAGS', '').split()
	env_aflags = os.environ.get('AFLAGS', '').split()

	defs = []
	for key in flags:
		t = '-D%s' % key.upper()
		if flags[key]:
			t += '=%s' % flags[key].upper()
		defs.append(t)

	for obj in resobj:
		obj_cflags = list(env_cflags)
		obj_cflags += resobj[obj].get('build-flags', [])
		obj_cflags += resobj[obj].get('cflags', [])

		obj_aflags = list(env_aflags)
		obj_aflags += resobj[obj].get('build-flags', [])
		obj_aflags += resobj[obj].get('aflags', [])

		objdir = resobj[obj].get('objdir', [DEFAULT_OBJDIR])[0]
		if not machine(vars, objdir):
			return False

		searchp = ['-I%s' % objdir]
		if 'search-path' in resobj[obj]:
			for p in resobj[obj]['search-path']:
				searchp.append('-I%s' % p)

		if obj.endswith('.a'):
			pcmd = "  AA      %s" % obj
			print(pcmd)
			link_objs.append(obj)
			continue

		o = None
		d = {'c': obj_cflags, 'S': obj_aflags}
		for x in d:
			p = "%s.%s" % (obj[:-2], x)
			if os.path.exists(p):
				o = p
				fl = d[x]
				break
		if not o:
			print("Source file not found for object: %s" % obj)
			continue

		ob = os.path.abspath(obj)
		objfile = "%s/%s" % (objdir, ob)
		link_objs.append(objfile)
		os.makedirs(os.path.dirname(objfile), exist_ok=True)

		cmd = [compiler] + defs + fl + searchp
		cmd += [o, '-c', '-o', objfile]
		pcmd = "  CC      %s" % o
		print(pcmd)
		if debug:
			print(" ".join(cmd))

		if run(compiler_fp, cmd) != 0:
			return False

	return True

def link(vars, link_objs, debug):

	args = vars.get('link', None)
	if not args:
		# Nothing to link
		return True

	if (len(args) % 2) != 0:
		print("Error: link directive must have even number of "
			"arguments: pairs of ldscript and output filename.")
		return False

	if 'ldadd' in vars:
		link_objs += vars['ldadd']

	ld = os.environ.get('LD', '')
	if ld:
		linker = ld
	else:
		cross_compile = os.environ.get('CROSS_COMPILE', '')
		linker = '%sld' % cross_compile

	linker_fp = find_elf(linker)
	if not linker_fp:
		print("Linker not found: %s" % linker)
		return False

	for ldscript, elf in zip(args[0::2], args[1::2]):
		os.makedirs(os.path.dirname(elf), exist_ok=True)
		cmd = [linker, "-T", ldscript] + link_objs + ["-o", elf]
		pcmd = "  LD      %s" % elf
		print(pcmd)
		if debug:
			print(" ".join(cmd))

		if run(linker_fp, cmd) != 0:
			return False

	return True

def build(resobj, flags, vars, debug=False):
	objs = []

	if not compile(resobj, flags, vars, objs, debug):
		return False

	if not link(vars, objs, debug):
		return False

	return True
