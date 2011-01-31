import os
import platform

env = Environment(ENV=os.environ)

if(platform.system() == "Linux"):
	env.Append(LIBS=['m'])

env.Append(LIBS=['c'])
env.Append(CCFLAGS=['-Wall','-pedantic','-g','-std=c99']) # ,'-Wextra'

# Update tag index
sources = Glob('*.[c]')
tags = env.Command('tags',sources,'ctags $SOURCES')
# AlwaysBuild(tags)
Default(tags)

# Clean up backup files
clean = env.Command('clean',None,'rm -f *~')

# OpenMP
env.Append(CFLAGS=['-fopenmp'])
openmp = env.Object('openmp_render.o','openmp_render.c')

# PThread
env.Append(CFLAGS=['-pthread'])
pthread = env.Object('pthread_render.o','pthread_render.c')

# Build target
target = env.Program('frac',['main.c','options.c','timer.c','render.c','ppm.c'] + openmp + pthread)

ppm = env.Command('output.ppm',target,'./$SOURCE -Svv -p 300 -I 10000 -G 800x800 -x 0.155 -X 0.160 -y 0.635 -Y 0.640 -F$TARGET')

env.Command('desktop.ppm',target, './$SOURCE -p 300 -I 4000 -G 1920x1080 -x 0.15306 -X 0.16194 -y 0.635 -Y 0.640 -S -T 4 -F$TARGET')


Default(target)

if(platform.system() == "Darwin"):
	view = env.Command('view',ppm,'open $SOURCES')
elif(platform.system() == "Linux"):
	view = env.Command('view',ppm,'xv $SOURCES &')

png = env.Command('output.png',ppm,'convert $SOURCES $TARGET')
