from subprocess import getstatusoutput
import sys
import os

""" This code is not in use """

def build(objdir, resobj, flags, vars):

	machine = vars.get('machine')
	if not machine:
		print('machine is not set')
		sys.exit(6)

	src = os.path.join('..', 'mdepx/kernel', machine, 'include')
	#if not os.path.exists(src):
	#	print("machine headers not found")
	#	sys.exit(7)

	dst = os.path.join(objdir, 'machine')
	if os.path.exists(dst):
		os.unlink(dst)
	os.symlink(src, dst)

	for obj in resobj:
		fl = ' '.join(resobj[obj].get('cflags'))
		for key in flags:
			v = flags[key]
			if v:
				fl += ' -D%s=%s' % (key.upper(), v.upper())
			else:
				fl += ' -D%s' % (key.upper())

		incs = '-I%s -I. -Imdepx/include -Imdepx/kernel -Imdepx/lib' %\
			(objdir)
		if 'incs' in resobj[obj]:
			for inc in resobj[obj]['incs']:
				incs += ' -Imdepx/%s ' % inc

		o = obj.replace(".o", ".c")
		if not os.path.exists(o):
			o = obj.replace(".o", ".S")

		compiler = '%sgcc' % vars.get('cross_compile')
		objfile = os.path.join(objdir, obj)
		os.makedirs(os.path.dirname(objfile), exist_ok=True)
		cmd = '%s %s %s %s -c -o %s' % \
			(compiler, fl, incs, o, objfile)

		pcmd = "  CC      %s" % o
		print(pcmd)
		status, output = getstatusoutput(cmd)
		if status != 0:
			print(output)
			sys.exit(7)

	linker = '%sld' % vars.get('cross_compile')
	objs = ''
	for o in resobj:
		objs += "%s " % os.path.join(objdir, o)
	if 'ldadd' in vars:
		for o in vars['ldadd']:
			objs += "%s " % o
	elf = os.path.join(objdir, "%s.elf" % vars.get('app'))
	cmd = "%s -T %s %s -o %s" % (linker, vars['ldscript'], objs, elf)
	pcmd = "  LD      %s" % elf
	print(pcmd)
	status, output = getstatusoutput(cmd)
	if status != 0:
		print(output)
