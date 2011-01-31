#include <omp.h>

#include "options.h"
#include "render.h"

void openmp_render(
		unsigned int *image, coord_t step_x, coord_t step_y,
		int x_start, int x_end, int y_start, int y_end) {
	int i;

	omp_set_num_threads(openmp);
#pragma omp parallell 
	{
#pragma omp	do
	{
		for(i = y_start; i < y_end; i++){
			render(image, step_x, step_y, x_start, x_end, i, i+1);
		}
	}
	}
}
