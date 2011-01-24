#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <stdio.h>

#define PROG_NAME "frac"

extern int iteration_max;
extern int threshold;

extern int res_x;
extern int res_y;

extern double min_x;
extern double max_x;

extern double min_y;
extern double max_y;

extern int palette_size;

void usage(FILE *stream);
void help(FILE *stream);

void parse_options(int argc, char *argv[]);

#endif
