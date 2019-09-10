from subprocess import getstatusoutput
import sys
import os

""" This code is not in use """

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

def build(objdir, resobj, flags, vars):

	machine(vars, objdir)

	link_objs = []

	for obj in resobj:
		fl = ' '.join(resobj[obj].get('cflags'))
		for key in flags:
			fl += ' -D%s' % (key.upper())
			if flags[key]:
				fl += '=%s' % flags[key].upper()

		incs = '-I%s' % objdir
		if 'incs' in resobj[obj]:
			for inc in resobj[obj]['incs']:
				incs += ' -I%s ' % inc

		o = obj.replace(".o", ".c")
		if not os.path.exists(o):
			o = obj.replace(".o", ".S")

		cc = resobj[obj].get('cross_compile') or ['']
		compiler = '%sgcc' % cc[0]
		ob = os.path.abspath(obj)
		objfile = "%s/%s" % (objdir, ob)
		link_objs.append(objfile)
		os.makedirs(os.path.dirname(objfile), exist_ok=True)
		cmd = '%s %s %s %s -c -o %s' % \
			(compiler, fl, incs, o, objfile)

		pcmd = "  CC      %s" % o
		print(cmd)
		status, output = getstatusoutput(cmd)
		if status != 0:
			print(output)
			sys.exit(7)

	linker = resobj[obj].get('cross_compile') or ['']
	linker = '%sld' % linker[0]
	if 'ldadd' in vars:
		for o in vars['ldadd']:
			link_objs.append(o)
	elf = os.path.join(objdir, "%s.elf" % vars.get('app'))
	cmd = "%s -T %s %s -o %s" % (linker, vars['ldscript'], " ".join(link_objs), elf)
	pcmd = "  LD      %s" % elf
	print(pcmd)
	status, output = getstatusoutput(cmd)
	if status != 0:
		print(output)
