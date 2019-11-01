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
	env_asflags = os.environ.get('ASFLAGS', '').split()

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

		obj_asflags = list(env_asflags)
		obj_asflags += resobj[obj].get('build-flags', [])
		obj_asflags += resobj[obj].get('asflags', [])

		objdir = resobj[obj].get('objdir', [DEFAULT_OBJDIR])[0]
		if not machine(vars, objdir):
			return False

		searchp = ['-I%s' % objdir]
		if 'search-path' in resobj[obj]:
			for p in resobj[obj]['search-path']:
				searchp.append('-I%s' % p)

		if obj.endswith('.a'):
			link_objs.append(obj)
			continuE

		o = None
		d = {'c': obj_cflags, 'S': obj_asflags}
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
	link_objs = []

	if not compile(resobj, flags, vars, link_objs, debug):
		return False

	if not link(vars, link_objs, debug):
		return False

	return True
