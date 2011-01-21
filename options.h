#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <stdio.h>

void usage(FILE *stream);
void help(FILE *stream);

void parse_options(int argc, char *argv[]);

#endif
