import os
import subprocess
import platform

# Local Builders #############################################################

def Valgrind(env, binary):
	return env.Command('%s.dSYM' % binary[0], binary, 'valgrind --dsymutil=yes ./$SOURCE -G60x40')
AddMethod(Environment,Valgrind)

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

env = Environment(ENV=os.environ)
lib = Environment(ENV=os.environ)

# Configuration ##############################################################

# if not env.GetOption('clean'):
conf = Configure(env,custom_tests = {'CheckCtags' : CheckCtags})

if not conf.CheckCC():
	print('Epic FAIL!!!')
	Exit(0)

if not conf.CheckFunc('usleep'):
	print('Did not find usleep()')
	Exit(0)

conf.env['havectags'] = conf.CheckCtags()

env = conf.Finish()

##############################################################################

if(platform.system() == "Linux"):
	env.Append(LIBS=['m'])

env.Append(LIBS=['c'])
env.Append(CCFLAGS=['-Wall','-pedantic','-g','-std=c99']) # ,'-Wextra'

# ILHeap #####################################################################
libirk_sources = ['libirk/' + f for f in ['ILHeap.c']]
if(platform.system() == 'Darwin'):
	libirk_tags = env.Tags('libirk/tags',libirk_sources,relative=True)
	Default(libirk_tags)
libirk = lib.Library('irk',libirk_sources)
Default(libirk)
env.Append(LIBS=['irk'],LIBPATH=['.'],CPPPATH=['libirk'])
test = lib.Program('libirk/heaptest.c')


# Update tag index ###########################################################
tags = env.Tags()
# AlwaysBuild(tags)
Default(tags)

# OpenMP #####################################################################
env.Append(CFLAGS=['-fopenmp'])
env.Append(LINKFLAGS=['-fopenmp'])
openmp = env.Object('openmp_render.o','openmp_render.c')

# PThread ####################################################################
env.Append(CFLAGS=['-pthread'])
env.Append(LINKFLAGS=['-pthread'])
pthread = [env.Object('pthread_render.o','pthread_render.c'),'pthread_common.c']

# Magic ######################################################################
magic = env.Object('magic_render.c')

# Build target ###############################################################
target = env.Program('frac',Split('main.c options.c timer.c render.c ppm.c') + openmp + pthread + magic)

ppm = env.Command('output.ppm',target,'./$SOURCE -Svv -p 300 -I 10000 -G 800x800 -x 0.155 -X 0.160 -y 0.635 -Y 0.640 -F$TARGET')
env.Command('desktop.ppm',target, './$SOURCE -p 300 -I 4000 -G 1920x1080 -x 0.15306 -X 0.16194 -y 0.635 -Y 0.640 -S -T 4 -F$TARGET --threads=2 -M 32')


Default(target)

if(platform.system() == "Darwin"):
	view = env.Command('view',ppm,'open $SOURCES')
elif(platform.system() == "Linux"):
	view = env.Command('view',ppm,'xv $SOURCES &')

png = env.Command('output.png',ppm,'convert $SOURCES $TARGET')

valgrind = env.Valgrind(target)
Clean(target,valgrind)

# Clean up backup files
clean = env.Command('clean',None,'rm -rf *~ **/*~ %s' % ' '.join(str(x) for x in valgrind))
if(env.GetOption('clean')):
	Default(clean)
