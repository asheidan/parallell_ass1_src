#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "options.h"
#include "render.h"
#include "timer.h"
#include "ppm.h"
#include "pthread_render.h"
#include "openmp_render.h"

void single_thread_render(unsigned int *image, coord_t step_x, coord_t step_y, int x_start, int x_end, int y_start, int y_end) {
	render(image,step_x, step_y,x_start,x_end,y_start,y_end);
}

int main(int argc, char *argv[]) {
	unsigned int *image;
	unsigned int
		buffer_size;
	double time;
	FILE *output;
	coord_t
		step_x,
		step_y;

	parse_options(argc,argv);
	if(verbosity) {
		fprintf(stderr,
				"Image: [I:%5u] [T:%3u] (%Lf,%Lf)-(%Lf,%Lf) [%ux%u]\n",
				iteration_max, threshold, min_x,min_y,max_x,max_y,res_x,res_y
			);
	}

	buffer_size = res_x * res_y;
	image = malloc(sizeof(unsigned int) * buffer_size);
	if(image == NULL) {
		perror(PROG_NAME);
		exit(3);
	}

	if(output_to_terminal) {
		output = stdout;
	}
	else {
		output = fopen(filename, "w");
		if(output == NULL) {
			fprintf(stderr,
					"%s: Opening file '%s' for output failed: %s\n",
					PROG_NAME, filename, strerror(errno)
				);
			exit(-4);
		}
	}


	step_x = ( max_x - min_x ) / res_x;
	min_x += step_x / 2.0;

	step_y = ( max_y - min_y ) / res_y;
	max_y -= step_y / 2.0;

	threshold = threshold * threshold;

	if(openmp) {
		if(verbosity > 0) {
			fprintf(stderr, "OpenMP render: %u threads\n", openmp);
		}
		if(print_time) timer_start();
		openmp_render(image,step_x, step_y,0,res_x,0,res_y);
	}
	else if(pthreads) {
		if(verbosity > 0) {
			fprintf(stderr, "Pthread render: %u threads\n", pthreads);
		}
		if(print_time) timer_start();
		pthread_render(image,step_x, step_y,0,res_x,0,res_y);
	}
	else {
		if(verbosity > 0) {
			fprintf(stderr, "Single thread render\n");
		}
		if(print_time) timer_start();
		single_thread_render(image,step_x, step_y,0,res_x,0,res_y);
	}

	if(print_time) {
		time = timer_check();
		fprintf(stdout, "%lf\n", time);
	}

	ppm_write(res_x, res_y, image, output);

	free(image);

	return 0;
}

