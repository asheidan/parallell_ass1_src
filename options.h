#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <stdio.h>
#include <stdbool.h>

#include "coord.h"

#define PROG_NAME "frac"

extern unsigned int iteration_max;
extern unsigned int threshold;

extern unsigned int res_x;
extern unsigned int res_y;

extern coord_t min_x;
extern coord_t max_x;

extern coord_t min_y;
extern coord_t max_y;

extern unsigned int palette_size;

extern bool output_to_terminal;
extern char *filename;
extern bool save_image;

extern bool continous_smoothing;
extern bool print_time;
extern unsigned int num_runs;
extern bool show_magic;

extern unsigned int verbosity;

extern bool pthreads;
extern bool openmp;

extern unsigned int num_threads;

extern unsigned int magic_size;

void usage(FILE *stream);
void help(FILE *stream);

void parse_options(int argc, char *argv[]);

#endif
