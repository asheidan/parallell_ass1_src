#ifndef __PPM_H__
#define __PPM_H__

#include <stdio.h>

#include "coord.h"

void ppm_write(unsigned int width, unsigned int height , pixel_t *buffer, FILE *stream);

#endif
