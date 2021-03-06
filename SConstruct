import os
import subprocess
import platform

import SCons

# Local Builders #############################################################

def Valgrind(env, binary):
	return env.Command('%s.dSYM' % binary[0], binary, 'valgrind --dsymutil=yes ./$SOURCE -G60x40')
AddMethod(Environment,Valgrind)

def CheckFlags(context,executable,flags):
	context.Message('Check if %s supports %s...' % (executable, flags))
	result = context.TryAction(action = '%s %s' % (executable, ' '.join(flags)))[0]
	context.Result(result)
	return result

def CheckCtags(context):
	context.Message('Checking for ctags...')
	result = context.TryAction(action = 'ctags --version')[0]
	context.Result(result)
	return result

def Tags(env, target = 'tags', sources = Glob('*.c'),relative = False):
	if(env['havectags']):
		additional_arguments = ''
		if relative:
			additional_arguments += '--tag-relative=yes'
		return env.Command(target,sources,'ctags %s -f $TARGET $SOURCES' % additional_arguments)
	else:
		return None
AddMethod(Environment,Tags)

##############################################################################
build = Dir('build')

VariantDir('build','src',duplicate = 0)
env = Environment(ENV=os.environ)
lib = Environment(ENV=os.environ)

print("Platform: %s(%s)" % ( platform.system(), env['PLATFORM'] ))
#if(SCons.__version__ >= '1.3.0'): print(env['PLATFORM_CPU'])

env.Append(CCFLAGS=['-Wall','-Wextra' ,'-pedantic','-g','-std=c99']) # 

# Configuration ##############################################################

# EnsureSConsVersion(1,3,0)
if not ( env.GetOption('clean') or 'clean' in COMMAND_LINE_TARGETS) :
	conf = Configure(env,custom_tests = {'CheckCtags' : CheckCtags, 'CheckFlags' : CheckFlags})

	if(SCons.__version__ >= '1.3.0'):
		if not conf.CheckCC():
			print('Epic FAIL!!!')
			Exit(0)

	if not conf.CheckLib('c'):
		print('You need LibC')
		Exit(0)
	
	if not conf.CheckCHeader('unistd.h'):
		print('Did not find unistd.h')
		Exit(0)
	if not conf.CheckType('useconds_t','#include <unistd.h>'):
		# Akka/Linux/Posix (Depends on std probably
		conf.env.Append(CPPDEFINES = {'_XOPEN_SOURCE':500})
	if not conf.CheckFunc('usleep'):
		print('Did not find usleep()')
		Exit(0)

	if not conf.CheckFunc('sqrtl'):
		print("\tSeems not to autolink math... -lm")
		if not( conf.CheckLib('m') and conf.CheckFunc('sqrtl') ):
			print('Did not find sqrtl()')
			Exit(0)

	if conf.CheckCtags():
		conf.env['havectags'] = True
		conf.env['havectagsrelative'] = conf.CheckFlags('ctags',['--tag-relative=yes','--version'])
	else:
		conf.env['havectags'] = False
		conf.env['havectagsrelative'] = False

	env = conf.Finish()
else:
	env['havectags'] = True
	env['havectagsrelative'] = True


##############################################################################


# ILHeap #####################################################################
libirk_sources = [os.path.join(str(build), 'libirk', f) for f in ['ILHeap.c']]
if(env['havectagsrelative']):
	libirk_tags = env.Tags('libirk/tags',libirk_sources,relative=True)
	# Default(libirk_tags)
libirk = lib.Library('irk',libirk_sources)
Default(libirk)
env.Append(LIBS=['irk'],LIBPATH=['.'],CPPPATH=['src/libirk'])
test = lib.Program('libirk/heaptest.c')


# Update tag index ###########################################################
tags = env.Tags()
# AlwaysBuild(tags)
# Default(tags)

# OpenMP #####################################################################
env.Append(CFLAGS=['-fopenmp'])
env.Append(LINKFLAGS=['-fopenmp'])
openmp = ['openmp_render.c']

# PThread ####################################################################
env.Append(CFLAGS=['-pthread'])
env.Append(LINKFLAGS=['-pthread'])
pthread = ['pthread_render.c','pthread_common.c']

# Magic ######################################################################
magic = ['magic_render.c']

# Build target ###############################################################
target = env.Program('frac',[os.path.join(str(build), str(x)) for x in (Split('main.c options.c timer.c render.c ppm.c') + openmp + pthread + magic)])

ppm = env.Command('output.ppm',target,'./$SOURCE -Svv -p 300 -I 10000 -G 800x800 -x 0.155 -X 0.160 -y 0.635 -Y 0.640 -F$TARGET')
env.Command('desktop.ppm',target, './$SOURCE -p 300 -I 4000 -G 1920x1080 -x 0.15306 -X 0.16194 -y 0.635 -Y 0.640 -S -T 4 -F$TARGET --threads=2 -M 32')


Default(target)

if(env['PLATFORM'] == "darwin"):
	view = env.Command('view',ppm,'open $SOURCES')
elif(env['PLATFORM'] == "posix"):
	view = env.Command('view',ppm,'xv $SOURCES &')

png = env.Command('output.png',ppm,'convert $SOURCES $TARGET')

valgrind = env.Valgrind(target)
Clean(target,valgrind)

# Clean up backup files
clean = env.Command('clean',None,'rm -rf *~ **/*~ %s' % ' '.join(str(x) for x in valgrind))
env.Clean('distclean',[target,libirk,'build','.sconf_temp','config.log','.sconsign.dblite'])
if(env.GetOption('clean') or ('clean' in COMMAND_LINE_TARGETS)):
	print("Cleaning")
	Clean('clean',[target,libirk])
	Default(clean)
