#include <omp.h>

#include "options.h"
#include "render.h"

void openmp_render(image_info_t *info, worker_task_t *task) {
	int
		x, y;
	coord_t
		cr, ci;

	omp_set_num_threads(num_threads);
		/*fprintf(stderr, "t: %d\n", omp_get_thread_num());*/
		/*fprintf(stderr, "T: %d\n", omp_get_num_threads());*/
#pragma omp parallel for private(x,y,cr,ci) schedule(dynamic)
	for(y = task->y_start; y < task->y_end; y++) {
		ci = max_y - info->step_y * y;
		for(x = task->x_start; x < task->x_end; x++) {
			cr = min_x + info->step_x * x;

			info->buffer[res_x * y + x] = iterate(cr,ci);
			/*image[res_x * y + x] = iteration_max - omp_get_thread_num();*/

		}
	}
}
