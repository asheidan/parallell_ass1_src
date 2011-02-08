#include "render.h"
#include "options.h"

#include <math.h>
#include <float.h>

pixel_t iterate(coord_t real, coord_t imag) {
	unsigned int
		iteration, mu;
	coord_t
		zr = 0, zi = 0, tmp,
		modulus;

	for(iteration = 0; (iteration < iteration_max) && ((zr*zr - zi*zi) <= threshold); iteration++) {
		// z = z^2 + c;
		tmp = zr*zr - zi*zi + real;
		zi = 2*zr*zi + imag;
		zr = tmp;
	}
	if(continous_smoothing && iteration < iteration_max) {
		
		/*
			tmp = zr*zr - zi*zi + cr;
			zi = 2*zr*zi + ci;
			zr = tmp;
			if(iteration < iteration_max) iteration++;
		*/

		if((zr*zr == INFINITY) && (zi * zi == INFINITY)) {
			modulus = sqrtl(fabsl(zr-zi));
		}
		else {
			modulus = sqrtl(fabsl(zr*zr - zi*zi));
		}
		mu = iteration - round(logl(fabsl(logl(modulus))) / logl(2.0));

		if(verbosity > 2) {
			fprintf(stdout, "fabsl: %Le\tzr %Le\tzi %Le\ti: %d\n", fabsl(zr*zr - zi*zi), zr*zr, zi*zi,iteration);
			fprintf(stdout, "mod: %Le\tmu: %u\t[zi:%Le + i%Le]\t[ci:%6.3Lf + i%6.3Lf]\n",modulus,mu,zi,zr,real,imag);
			fprintf(stdout, "sqrt: %Le\n\n", sqrtl(fabsl(zr*zr - zi*zi)));
		}
		if(iteration < iteration_max && palette_size > iteration_max) {
			return (mu * palette_size / iteration_max);
		}
		else {
			return mu;
		}
	}
	else {
		if(iteration < iteration_max && palette_size > iteration_max) {
			return iteration * palette_size / iteration_max;
		}
		else {
			return iteration;
		}
	}
}

void render(image_info_t *info,worker_task_t *t) {
	unsigned int
		x, y;
	coord_t
		cr, ci;

	for(y = t->y_start; y < t->y_end; y++) {
		ci = max_y - info->step_y * y;
		for(x = t->x_start; x < t->x_end; x++) {
			cr = min_x + info->step_x * x;

			info->buffer[res_x * y + x] = iterate(cr,ci);
		}
	}
}
