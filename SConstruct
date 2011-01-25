import os
import platform

env = Environment(ENV=os.environ)

if(platform.system() == "Linux"):
	env.Append(LIBS=['m'])
	env.Append(CFLAGS=['-std=c99'])

env.Append(LIBS=['c'])
env.Append(CFLAGS=['-Wall','-g'])

# Update tag index
sources = Glob('*.[c]')
tags = env.Command('tags',sources,'ctags $SOURCES')
# AlwaysBuild(tags)
Default(tags)

# Clean up backup files
clean = env.Command('clean',None,'rm -f *~')

# Build target
target = env.Program('frac',['main.c','options.c','timer.c','render.c','ppm.c'])

ppm = env.Command('output.ppm',target,'./$SOURCE -Cvv -P 300 -I 10000 -G 800x800 -x 0.155 -X 0.160 -y 0.635 -Y 0.640 -F$TARGET')

env.Command('desktop.ppm',target, './$SOURCE -P 300 -I 4000 -G 1920x1080 -x 0.15306 -X 0.16194 -y 0.635 -Y 0.640 -C -T 4 -F$TARGET')


Default(target)

if(platform.system() == "Darwin"):
	view = env.Command('view',ppm,'open $SOURCES')
elif(platform.system() == "Linux"):
	view = env.Command('view',ppm,'xv $SOURCES')
