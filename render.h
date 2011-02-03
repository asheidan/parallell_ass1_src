#ifndef __RENDER_H__
#define __RENDER_H__

#include "coord.h"

pixel_t iterate(coord_t real, coord_t imag);
void render(pixel_t *buffer, coord_t step_x, coord_t step_y, int x_start, int x_end, int y_start, int y_end);

#endif
