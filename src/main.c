#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "options.h"
#include "render.h"
#include "timer.h"
#include "ppm.h"
#include "pthread_common.h"
#include "pthread_render.h"
#include "openmp_render.h"
#include "magic_render.h"

void single_thread_render(image_info_t *info, worker_task_t *t) {
	render(info,t);
}

int main(int argc, char *argv[]) {
	unsigned int
		buffer_size;
	double time;
	FILE *output;
	image_info_t
		info;
	worker_task_t
		task;

	parse_options(argc,argv);
	if(verbosity) {
		fprintf(stderr,
				"Image: [I:%5u] [T:%3u] (%Lf,%Lf)-(%Lf,%Lf) [%ux%u]\n",
				iteration_max, threshold, min_x,min_y,max_x,max_y,res_x,res_y
			);
	}

	buffer_size = res_x * res_y;
	info.buffer = malloc(sizeof(pixel_t) * buffer_size);
	if(info.buffer == NULL) {
		perror(PROG_NAME);
		fprintf(stderr, "info.buffer\n");
		exit(3);
	}
	memset(info.buffer,0,sizeof(pixel_t) * buffer_size);

	if(save_image) {
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
	}


	info.step_x = ( max_x - min_x ) / res_x;
	min_x += info.step_x / 2.0;

	info.step_y = ( max_y - min_y ) / res_y;
	max_y -= info.step_y / 2.0;

	threshold = threshold * threshold;

	task.x_start = 0;
	task.x_end = res_x;
	task.y_start = 0;
	task.y_end = res_y;

	if(magic_size > 0) {
		if(verbosity > 0) {
			fprintf(stderr, "Magic render: %u threads\tmin size: %u\n", num_threads, magic_size);
		}
		if(print_time) timer_start();
		for(;num_runs > 0; num_runs--) magic_render( &info, &task );
	}
	else if(openmp) {
		if(verbosity > 0) {
			fprintf(stderr, "OpenMP render: %u threads\n", num_threads);
		}
		if(print_time) timer_start();
		for(;num_runs > 0; num_runs--) openmp_render( &info, &task );
	}
	else if(pthreads) {
		if(verbosity > 0) {
			fprintf(stderr, "Pthread render: %u threads\n", num_threads);
		}
		if(print_time) timer_start();
		for(;num_runs > 0; num_runs--) pthread_render( &info, &task );
	}
	else {
		if(verbosity > 0) {
			fprintf(stderr, "Single thread render\n");
		}
		if(print_time) timer_start();
		for(;num_runs > 0; num_runs--) single_thread_render( &info, &task );
	}

	if(print_time) {
		time = timer_check();
		fprintf(stdout, "%lf\n", time);
	}

	if(save_image) {
		ppm_write(res_x, res_y, info.buffer, output);
	}

	free(info.buffer);

	return 0;
}

