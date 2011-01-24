#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <stdio.h>
#include <stdbool.h>

#define PROG_NAME "frac"

extern unsigned int iteration_max;
extern unsigned int threshold;

extern unsigned int res_x;
extern unsigned int res_y;

extern long double min_x;
extern long double max_x;

extern long double min_y;
extern long double max_y;

extern unsigned int palette_size;

extern bool output_to_terminal;
extern char *filename;

void usage(FILE *stream);
void help(FILE *stream);

void parse_options(int argc, char *argv[]);

#endif
