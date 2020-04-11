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

from debug import warn, err
import multiprocessing
import os

DEFAULT_OBJDIR = 'obj'

def machine(vars, objdir):
	machine = vars.get('machine')
	if not machine:
		err("machine directive is not set")
		return False

	m = os.path.abspath(machine)
	if not os.path.exists(m):
		err("machine headers not found at path: %s" % m)
		return False

	dst = os.path.join(objdir, 'machine')
	#if os.path.lexists(dst):
	#	os.unlink(dst)
	if not os.path.lexists(dst):
		os.symlink(m, dst)

	return True

def find_elf(name):
	name = os.path.expanduser(name)
	for path in os.environ["PATH"].split(os.pathsep):
		elf = os.path.join(path, name)
		if os.path.exists(elf):
			return elf
	return None

def run(print_cmd, debug, compiler_fp, cmd):
	print(print_cmd)
	if debug:
		print(" ".join(cmd))

	pid = os.fork()
	if pid == 0:
		os.execve(compiler_fp, (cmd), os.environ)

	t = os.waitpid(pid, 0)
	return t[1]

def compile(r, link_objs, debug, parallel):

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
	flags = r['flags']
	for key in flags:
		t = '-D%s' % key.upper()
		if flags[key]:
			t += '=%s' % flags[key].upper()
		defs.append(t)

	archive_objs = []

	cpu_count = 1
	if (parallel):
		cpu_count = multiprocessing.cpu_count()

	pool = multiprocessing.Pool(processes = cpu_count)

	res = []
	vars = r['vars']
	resobj = r['resobj']
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
			archive_objs.append(obj)
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
			err("Source code (or archive file) not found"
			    " for the object: %s" % obj)
			return False

		ob = os.path.abspath(obj)
		objfile = "%s/%s" % (objdir, ob)
		link_objs.append(objfile)
		os.makedirs(os.path.dirname(objfile), exist_ok=True)

		cmd = [compiler] + defs + fl + searchp
		cmd += [o, '-c', '-o', objfile]
		pcmd = "  CC      %s" % o
		if debug:
			print(" ".join(cmd))

		v = pool.apply_async(run, (pcmd, debug, compiler_fp, cmd))
		res.append(v)

	# Wait completion
	for x in res:
		x.get()

	# Add all the archives to the tail
	link_objs += archive_objs

	return True

def link(vars, link_objs, debug):

	args = vars.get('link', None)
	if not args:
		# Nothing to link
		return True

	if (len(args) % 2) != 0:
		err("link directive must have even number of "
		    "arguments: pairs of ldscript and output filename.")
		return False

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
		if run(pcmd, debug, linker_fp, cmd) != 0:
			return False

	return True

def build(result, debug=False, parallel=False):
	objs = []

	if not compile(result, objs, debug, parallel):
		return False

	if not link(result['vars'], objs, debug):
		return False

	return True
