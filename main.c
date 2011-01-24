#include <stdio.h>
#include <stdlib.h>

#include "options.h"
#include "render.h"
#include "ppm.h"

int main(int argc, char *argv[]) {
	int *image;
	int
		buffer_size,
		iteration_count;

	parse_options(argc,argv);
	fprintf(stderr,
			"Image: [I:%5d] [T:%3d] (%lf,%lf)-(%lf,%lf) [%dx%d]\n",
			iteration_max, threshold, min_x,min_y,max_x,max_y,res_x,res_y
		);

	buffer_size = res_x * res_y;
	image = malloc(sizeof(int) * buffer_size);
	if(image == NULL) {
		perror(PROG_NAME);
		exit(3);
	}

	render(buffer_size, image);

	ppm_write(res_x, res_y, image, iteration_count);

	return 0;
}

