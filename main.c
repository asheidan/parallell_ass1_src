#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "options.h"
#include "render.h"
#include "ppm.h"

int main(int argc, char *argv[]) {
	unsigned int *image;
	unsigned int
		buffer_size;
	FILE *output;

	parse_options(argc,argv);
	fprintf(stderr,
			"Image: [I:%5u] [T:%3u] (%Lf,%Lf)-(%Lf,%Lf) [%ux%u]\n",
			iteration_max, threshold, min_x,min_y,max_x,max_y,res_x,res_y
		);

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

	render(buffer_size, image);

	ppm_write(res_x, res_y, image, output);

	free(image);

	return 0;
}

