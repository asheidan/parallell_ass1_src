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
target = env.Program('frac',['main.c','options.c','render.c','ppm.c'])
Default(target)

ppm = env.Command('test.ppm',target,'./$SOURCE > $TARGET')

