#include "render.h"
#include "options.h"

#include <math.h>
#include <float.h>

void render(unsigned int *buffer, coord_t step_x, coord_t step_y, int x_start, int x_end, int y_start, int y_end) {
	int
		iteration,
		x, y;
	coord_t
		T,
		zr, zi, cr, ci,
		tmp,
		modulus, mu;

	T = threshold * threshold;

	for(y = y_start; y < y_end; y++) {
		ci = max_y - step_y * y;
		for(x = x_start; x < x_end; x++) {
			zr = zi = 0;
			cr = min_x + step_x * x;
			for(iteration = 0; (iteration < iteration_max) && ((zr*zr - zi*zi) <= T); iteration++) {
				// z = z^2 + c;
				tmp = zr*zr - zi*zi + cr;
				zi = 2*zr*zi + ci;
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
				mu = iteration - logl(fabsl(logl(modulus))) / logl(2.0);

				if(verbosity > 2) {
					fprintf(stdout, "fabsl: %Le\tzr %Le\tzi %Le\ti: %d\n", fabsl(zr*zr - zi*zi), zr*zr, zi*zi,iteration);
					fprintf(stdout, "mu: %Le\tmod: %Le\t[zi:%Le + i%Le]\t[ci:%6.3Lf + i%6.3Lf]\n",mu,modulus,zi,zr,ci,cr);
					fprintf(stdout, "sqrt: %Le\n\n", sqrtl(fabsl(zr*zr - zi*zi)));
				}
				if(iteration < iteration_max && palette_size > iteration_max) {
					buffer[res_x * y + x] = mu * palette_size / iteration_max;
				}
				else {
					buffer[res_x * y + x] = mu;
				}
			}
			else {
			  	if(iteration < iteration_max && palette_size > iteration_max) {
					buffer[res_x * y + x] = iteration * palette_size / iteration_max;
				}
				else {
					buffer[res_x * y + x] = iteration;
				}
			}
			/*
			fprintf(stderr,
					"[%3d,%3d] [i:%4d] [c: %lf + i*%lf]\n",
					x, y, iteration, cr, ci);
			*/
		}
	}
}
