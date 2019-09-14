import sys
import os

DEFAULT_OBJDIR = 'obj'

def machine(vars, objdir):
	machine = vars.get('machine')
	if not machine:
		print('machine is not set')
		sys.exit(6)

	m = os.path.abspath(machine)
	if not os.path.exists(m):
		print("machine headers not found at path: %s" % m)
		sys.exit(7)

	dst = os.path.join(objdir, 'machine')
	if os.path.exists(dst):
		os.unlink(dst)
	os.symlink(m, dst)

def find_elf(name):
	for path in os.environ["PATH"].split(os.pathsep):
		elf = os.path.join(path, name)
		if os.path.exists(elf):
			return elf
	return None

def run(cmd, args):
	pid = os.fork()
	if pid == 0:
		os.execve(cmd, args, os.environ)
		return

	t = os.waitpid(pid, 0)
	return t[1]

def build(resobj, flags, vars):

	link_objs = []
	ldadd = []

	cross_compile = os.environ.get('CROSS_COMPILE', '')

	cc = os.environ.get('CC', '')
	if cc:
		compiler = cc
	else:
		compiler = '%sgcc' % cross_compile
	compiler_fp = find_elf(compiler)
	if not compiler_fp:
		print("compiler not found in PATH: %s" % compiler)
		return

	cflags = os.environ.get('CFLAGS', '').split()
	for key in flags:
		t = '-D%s' % key.upper()
		if flags[key]:
			t += '=%s' % flags[key].upper()
		cflags.append(t)

	for obj in resobj:
		fl = cflags + resobj[obj].get('cflags', [])

		objdir = resobj[obj].get('objdir', [DEFAULT_OBJDIR])[0]
		machine(vars, objdir)

		incs = ['-I%s' % objdir]
		if 'incs' in resobj[obj]:
			for inc in resobj[obj]['incs']:
				incs.append('-I%s' % inc)

		if obj.endswith('.a'):
			ldadd.append(obj)
			continue

		o = None
		for x in ['c', 'S']:
			p = "%s.%s" % (obj[:-2], x)
			if os.path.exists(p):
				o = p
				break
		if not o:
			print("Source file not found for object: %s" % o)

		ob = os.path.abspath(obj)
		objfile = "%s/%s" % (objdir, ob)
		link_objs.append(objfile)
		os.makedirs(os.path.dirname(objfile), exist_ok=True)

		cmd = [compiler] + fl + incs + [o, '-c', '-o', objfile]
		pcmd = "  CC      %s" % o
		print(pcmd)

		if run(compiler_fp, cmd) != 0:
			return

	link_objs += ldadd
	if 'ldadd' in vars:
		link_objs += vars['ldadd']

	ldscript = vars.get('ldscript', None)
	if not ldscript:
		print("Error: ldscript is not provided. Can't link")
		return

	ld = os.environ.get('LD', '')
	if ld:
		linker = ld
	else:
		linker = '%sld' % cross_compile
	linker_fp = find_elf(linker)
	if not linker_fp:
		print("Linker not found: %s" % linker)
		return

	elf = os.path.join(objdir, "%s.elf" % vars.get('app', 'app'))

	cmd = [linker, "-T", ldscript] + link_objs + ["-o", elf]
	pcmd = "  LD      %s" % elf
	print(pcmd)

	if run(linker_fp, cmd) != 0:
		return
