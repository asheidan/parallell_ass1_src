#include <omp.h>

#include "options.h"
#include "render.h"

void openmp_render(
		unsigned int *image, coord_t step_x, coord_t step_y,
		int x_start, int x_end, int y_start, int y_end) {
	int
		x, y;
	coord_t
		cr, ci;

	omp_set_num_threads(num_threads);
		/*fprintf(stderr, "t: %d\n", omp_get_thread_num());*/
		/*fprintf(stderr, "T: %d\n", omp_get_num_threads());*/
#pragma omp parallel for private(x,y,cr,ci) schedule(dynamic)
	for(y = y_start; y < y_end; y++) {
		ci = max_y - step_y * y;
		for(x = x_start; x < x_end; x++) {
			cr = min_x + step_x * x;

			image[res_x * y + x] = iterate(cr,ci);
			/*image[res_x * y + x] = iteration_max - omp_get_thread_num();*/

		}
	}
}
