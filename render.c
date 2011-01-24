#include "render.h"
#include "options.h"

int render(int buffer_size, int *buffer) {
	int
		iteration,
		iteration_count,
		x, y;
	double
		zr, zi, cr, ci,
		tmp,
		interval_x,
		interval_y;


	if(res_x > 1) {
		interval_x = ( max_x - min_x ) / res_x;
	}
	else {
		interval_x = ( max_x - min_x );
	}
	min_x += interval_x / 2.0;

	if(res_y > 1) {
		interval_y = ( max_y - min_y ) / res_y;
	}
	else {
		interval_y = ( max_y - min_y );
	}
	max_y -= interval_y / 2.0;

	threshold = threshold * threshold;

	iteration_count = 0;
	for(y = 0; y < res_y; y++) {
		ci = max_y - interval_y * y;
		for(x = 0; x < res_x; x++) {
			zr = zi = 0;
			cr = min_x + interval_x * x;
			for(iteration = 0; iteration < iteration_max && (zr*zr - zi*zi) <= threshold; iteration++) {
				// z = z^2 + c;
				tmp = zr*zr - zi*zi + cr;
				zi = 2*zr*zi + ci;
				zr = tmp;
			}
			buffer[res_x * y + x] = iteration;
			if(iteration > iteration_count && iteration < iteration_max) {
				iteration_count = iteration;
			}
			/*
			fprintf(stderr,
					"[%3d,%3d] [i:%4d] [c: %lf + i*%lf]\n",
					x, y, iteration, cr, ci);
			*/
		}
	}

	return iteration_count;
}
