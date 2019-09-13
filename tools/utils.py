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

	cross_compile = os.environ.get('CROSS_COMPILE', '')
	cc = os.environ.get('CC', '')
	if cc:
		compiler = cc
	else:
		compiler = '%sgcc' % cross_compile

	cflags = os.environ.get('CFLAGS', '')

	for obj in resobj:
		fl = cflags + ' ' + ' '.join(resobj[obj].get('cflags', []))
		for key in flags:
			fl += ' -D%s' % (key.upper())
			if flags[key]:
				fl += '=%s' % flags[key].upper()

		objdir = resobj[obj].get('objdir', [DEFAULT_OBJDIR])[0]
		machine(vars, objdir)

		incs = '-I%s' % objdir
		if 'incs' in resobj[obj]:
			for inc in resobj[obj]['incs']:
				incs += ' -I%s ' % inc

		o = obj.replace(".o", ".c")
		if not os.path.exists(o):
			o = obj.replace(".o", ".S")

		ob = os.path.abspath(obj)
		objfile = "%s/%s" % (objdir, ob)
		link_objs.append(objfile)
		os.makedirs(os.path.dirname(objfile), exist_ok=True)
		cmd = '%s %s %s %s -c -o %s' % \
			(compiler, fl, incs, o, objfile)
		pcmd = "  CC      %s" % o
		print(pcmd)

		p = find_elf(compiler)
		if not p:
			return
		if run(p, cmd.split()) != 0:
			return

	if 'ldadd' in vars:
		for o in vars['ldadd']:
			link_objs.append(o)

	ldscript = vars.get('ldscript', '')
	if not ldscript:
		print("Error: ldscript is not provided")
		return

	ld = os.environ.get('LD', '')
	if ld:
		linker = ld
	else:
		linker = '%sld' % cross_compile

	fp = find_elf(linker)
	if not fp:
		print("Linker not found: %s" % linker)
		return

	elf = os.path.join(objdir, "%s.elf" % vars.get('app', 'app'))
	cmd = "%s -T %s %s -o %s" % \
		(linker, vars['ldscript'], " ".join(link_objs), elf)
	pcmd = "  LD      %s" % elf
	print(pcmd)

	if run(fp, cmd.split()) != 0:
		return
