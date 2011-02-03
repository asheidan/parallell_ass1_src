import os
import platform

env = Environment(ENV=os.environ)

if(platform.system() == "Linux"):
	env.Append(LIBS=['m'])

env.Append(LIBS=['c'])
env.Append(CCFLAGS=['-Wall','-pedantic','-g','-std=c99']) # ,'-Wextra'

# ILHeap
libirk_sources = ['libirk/' + f for f in ['ILHeap.c']]
libirk_tags = env.Command('libirk/tags',libirk_sources,'ctags -f $TARGET --tag-relative=yes $SOURCES')
Default(libirk_tags)
libirk = env.Library('irk',libirk_sources)
Default(libirk)
env.Append(LIBS=['irk'],LIBPATH=['.'],CPPPATH=['libirk'])
test = env.Program('libirk/heaptest.c')


# Update tag index
tags = env.Command('tags',Glob('*.[c]'),'ctags $SOURCES')
# AlwaysBuild(tags)
Default(tags)

# Clean up backup files
clean = env.Command('clean',None,'rm -f *~')

# OpenMP
env.Append(CFLAGS=['-fopenmp'])
env.Append(LINKFLAGS=['-fopenmp'])
openmp = env.Object('openmp_render.o','openmp_render.c')

# PThread
env.Append(CFLAGS=['-pthread'])
env.Append(LINKFLAGS=['-pthread'])
pthread = [env.Object('pthread_render.o','pthread_render.c'),'pthread_common.c']

# Magic
magic = env.Object('magic_render.c')

# Build target
target = env.Program('frac',['main.c','options.c','timer.c','render.c','ppm.c'] + openmp + pthread + magic)

ppm = env.Command('output.ppm',target,'./$SOURCE -Svv -p 300 -I 10000 -G 800x800 -x 0.155 -X 0.160 -y 0.635 -Y 0.640 -F$TARGET')
env.Command('desktop.ppm',target, './$SOURCE -p 300 -I 4000 -G 1920x1080 -x 0.15306 -X 0.16194 -y 0.635 -Y 0.640 -S -T 4 -F$TARGET --openmp=2')


Default(target)

if(platform.system() == "Darwin"):
	view = env.Command('view',ppm,'open $SOURCES')
elif(platform.system() == "Linux"):
	view = env.Command('view',ppm,'xv $SOURCES &')

png = env.Command('output.png',ppm,'convert $SOURCES $TARGET')


