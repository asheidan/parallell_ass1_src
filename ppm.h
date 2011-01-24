#ifndef __PPM_H__
#define __PPM_H__

#include <stdio.h>

void ppm_write(int width, int height, unsigned int *buffer, FILE *stream);

#endif
