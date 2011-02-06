#include "ppm.h"
#include <stdlib.h>

#include "options.h"

#define R 0
#define G 1
#define B 2
#define palette_location(X) (((X) * size) / steps + 1)

typedef unsigned char color_component_t;
typedef color_component_t color_t[3];

void fprintcolor(FILE *stream, color_t color) {
	fprintf(stderr, "C: 0x%02x%02x%02x\t%3u %3u %3u \n",
			color[R],
			color[G],
			color[B],
			color[R],
			color[G],
			color[B]
		);
}
color_t *palette(unsigned int size) {
	color_t
		*colors;
	unsigned int
		i, steps,
		blue_low,
		red_low;
	color_component_t
		red = 0,
		green = 0,
		blue = 0;

	colors = malloc(sizeof(color_t) * (size + 1));
	if(colors == NULL) {
		perror(PROG_NAME);
		fprintf(stderr, "colors\n");
		exit(3);
	}

	colors[0][R] = red;
	colors[0][G] = green;
	colors[0][B] = blue;

	blue_low = 50;
	red_low = 100;
	steps = 1275;

	/* Black to blue */
	red = 0;
	green = 0;
	blue = blue_low - 1;
	for(i = 1; blue < 255; i++ ) {
		colors[palette_location(i)][R] = red;
		colors[palette_location(i)][G] = green;
		colors[palette_location(i)][B] = ++blue;
		
		if(verbosity > 2) {
			fprintf(stderr, "%4d ", i);
			fprintcolor(stderr, colors[palette_location(i)]);
		}
	}
	/* Blue to white */
	for(; red < 255 && green < 255; i++) {
		colors[palette_location(i)][R] = ++red;
		colors[palette_location(i)][G] = ++green;
		colors[palette_location(i)][B] = blue;
		
		if(verbosity > 2) {
			fprintf(stderr, "%4d ", i);
			fprintcolor(stderr, colors[palette_location(i)]);
		}
	}
	/* White to yellow */
	for(; blue > 0; i++) {
		colors[palette_location(i)][R] = red;
		colors[palette_location(i)][G] = green;
		colors[palette_location(i)][B] = --blue;
		
		if(verbosity > 2) {
			fprintf(stderr, "%4d ", i);
			fprintcolor(stderr, colors[palette_location(i)]);
		}
	}
	/* Yellow to red */
	for(; green > 0; i++) {
		colors[palette_location(i)][R] = red;
		colors[palette_location(i)][G] = --green;
		colors[palette_location(i)][B] = blue;
		
		if(verbosity > 2) {
			fprintf(stderr, "%4d ", i);
			fprintcolor(stderr, colors[palette_location(i)]);
		}
	}
	/* Red to Dark */
	for(; red > red_low; i++) {
		colors[palette_location(i)][R] = --red;
		colors[palette_location(i)][G] = green;
		colors[palette_location(i)][B] = blue;
		
		if(verbosity > 2) {
			fprintf(stderr, "%4d ", i);
			fprintcolor(stderr, colors[palette_location(i)]);
		}
	}
	/* Dark Red to purple */
	for(; blue < blue_low; i++) {
		colors[palette_location(i)][R] = red;
		colors[palette_location(i)][G] = green;
		colors[palette_location(i)][B] = ++blue;
		
		if(verbosity > 2) {
			fprintf(stderr, "%4d ", i);
			fprintcolor(stderr, colors[palette_location(i)]);
		}
	}
	/* Purple to blue */
	for(; red > 1; i++) {
		colors[palette_location(i)][R] = --red;
		colors[palette_location(i)][G] = green;
		colors[palette_location(i)][B] = blue;
		
		if(verbosity > 2) {
			fprintf(stderr, "%4d ", i);
			fprintcolor(stderr, colors[palette_location(i)]);
		}
	}

	/*fprintf(stderr, "0x%02x%02x%02x\n", colors[i][R], colors[i][G], colors[i][B]);*/

	return colors;
}

void ppm_write(unsigned int width, unsigned int height , pixel_t *buffer, FILE *stream) {
	unsigned int buffer_size,i;
	pixel_t tmp;
	buffer_size = width * height;

	color_t
		*colors;

	colors = palette(palette_size);

	fprintf(stream, "P6\n");
	if(verbosity > 1) fprintf(stream, "%d %d\n%d\n", width, height+5, 255);
	else fprintf(stream, "%d %d\n%d\n", width, height, 255);

	for(i = 0; i < buffer_size; i++) {
		tmp = buffer[i];
		if(tmp == iteration_max) {
			fwrite(colors[0], sizeof(color_t), 1, stream);
		}
		else {
			fwrite(colors[tmp % (palette_size) + 1], sizeof(color_t), 1, stream);
		}
	}

	if(verbosity > 1) {
		for(tmp= 0; tmp < 5; tmp++) {
			for(i = 0; i < width; i++) {
				fwrite(colors[i * palette_size / width], sizeof(color_t), 1, stream);
			}
		}
	}

	free(colors);
}
