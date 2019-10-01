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
		return False

	cflags = os.environ.get('CFLAGS', '').split()
	asflags = os.environ.get('ASFLAGS', '').split()
	if not asflags:
		asflags = cflags

	defs = []
	for key in flags:
		t = '-D%s' % key.upper()
		if flags[key]:
			t += '=%s' % flags[key].upper()
		defs.append(t)

	for obj in resobj:
		cflags += resobj[obj].get('cflags', [])
		asflags += resobj[obj].get('asflags', [])

		objdir = resobj[obj].get('objdir', [DEFAULT_OBJDIR])[0]
		if not machine(vars, objdir):
			return False

		incs = ['-I%s' % objdir]
		if 'incs' in resobj[obj]:
			for inc in resobj[obj]['incs']:
				incs.append('-I%s' % inc)

		if obj.endswith('.a'):
			ldadd.append(obj)
			continue

		o = None
		d = {'c': cflags, 'S': asflags}
		for x in d:
			p = "%s.%s" % (obj[:-2], x)
			if os.path.exists(p):
				o = p
				fl = d[x]
				break
		if not o:
			print("Source file not found for object: %s" % obj)

		ob = os.path.abspath(obj)
		objfile = "%s/%s" % (objdir, ob)
		link_objs.append(objfile)
		os.makedirs(os.path.dirname(objfile), exist_ok=True)

		cmd = [compiler] + defs + fl + incs + [o, '-c', '-o', objfile]
		pcmd = "  CC      %s" % o
		print(pcmd)
		#print(" ".join(cmd))

		if run(compiler_fp, cmd) != 0:
			return False

	link_objs += ldadd
	if 'ldadd' in vars:
		link_objs += vars['ldadd']

	ldscript = vars.get('ldscript', None)
	if not ldscript:
		print("Error: ldscript is not provided. Can't link")
		return False

	ld = os.environ.get('LD', '')
	if ld:
		linker = ld
	else:
		linker = '%sld' % cross_compile
	linker_fp = find_elf(linker)
	if not linker_fp:
		print("Linker not found: %s" % linker)
		return False

	elf = os.path.join(objdir, "%s.elf" % vars.get('app', 'app'))

	cmd = [linker, "-T", ldscript] + link_objs + ["-o", elf]
	pcmd = "  LD      %s" % elf
	print(pcmd)
	#print(" ".join(cmd))

	if run(linker_fp, cmd) != 0:
		return False

	return True
