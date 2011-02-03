#ifndef __MAGIC_RENDER_H__
#define __MAGIC_RENDER_H__

#include "coord.h"

void magic_render(
		pixel_t *image, coord_t step_x, coord_t step_y,
		int x_start, int x_end, int y_start, int y_end);

#endif
