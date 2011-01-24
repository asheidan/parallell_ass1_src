#include "render.h"
#include "options.h"

#include <math.h>
#include <float.h>

void render(unsigned int buffer_size, unsigned int *buffer) {
	int
		iteration,
		x, y;
	long double
		T,
		zr, zi, cr, ci,
		tmp,
		interval_x,
		interval_y,
		modulus, mu;


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

	T = threshold * threshold;

	for(y = 0; y < res_y; y++) {
		ci = max_y - interval_y * y;
		for(x = 0; x < res_x; x++) {
			zr = zi = 0;
			cr = min_x + interval_x * x;
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
