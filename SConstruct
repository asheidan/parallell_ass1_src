env = Environment()

env.Append(CFLAGS=['-Wall','-g'])
env.Append(LIBS=['c'])

# Update tag index
sources = Glob('*.[c]')
tags = env.Command('tags',sources,'ctags $SOURCES')
# AlwaysBuild(tags)
Default(tags)

# Clean up backup files
clean = env.Command('clean',None,'rm -f *~')

# Build target
target = env.Program('frac',['main.c','options.c','timer.c','render.c','ppm.c'])

ppm = env.Command('output.ppm',target,'./$SOURCE -C -P 300 -I 10000 -G 800x800 -x 0.155 -X 0.160 -y 0.635 -Y 0.640 -F$TARGET')

Default(target)
