from subprocess import getstatusoutput
from distutils import spawn
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

def build(resobj, flags, vars):

	link_objs = []

	cross_compile = os.environ.get('CROSS_COMPILE', '')
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

		compiler = '%sgcc' % cross_compile
		ob = os.path.abspath(obj)
		objfile = "%s/%s" % (objdir, ob)
		link_objs.append(objfile)
		os.makedirs(os.path.dirname(objfile), exist_ok=True)
		cmd = '%s %s %s %s -c -o %s' % \
			(compiler, fl, incs, o, objfile)
		pcmd = "  CC      %s" % o
		print(pcmd)
		status, output = getstatusoutput(cmd)
		if status != 0:
			print(output)
			sys.exit(7)

	linker = '%sld' % cross_compile
	if not spawn.find_executable(linker):
		print("Linker not found: %s" % linker)
		return

	if 'ldadd' in vars:
		for o in vars['ldadd']:
			link_objs.append(o)
	elf = os.path.join(objdir, "%s.elf" % vars.get('app', 'app'))
	cmd = "%s -T %s %s -o %s" % \
		(linker, vars['ldscript'], " ".join(link_objs), elf)
	pcmd = "  LD      %s" % elf
	print(pcmd)
	status, output = getstatusoutput(cmd)
	if status != 0:
		print(output)
