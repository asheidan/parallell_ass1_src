#include "ppm.h"
#include <stdlib.h>

#include "options.h"

#define R 0
#define G 1
#define B 2

typedef unsigned char color_t[3];

color_t *palette(unsigned int size) {
	color_t
		*colors;
	unsigned int
		i, steps;
	colors = malloc(sizeof(color_t) * (size + 1));
	if(colors == NULL) {
		perror(PROG_NAME);
		exit(3);
	}

	colors[0][R] = 0;
	colors[0][G] = 0;
	colors[0][B] = 0;

	steps = 1275;

	for(i = 1; i < 255; i++ ) {
		colors[(i * palette_size) / steps + 1][R] = 1;
		colors[(i * palette_size) / steps + 1][G] = 1;
		colors[(i * palette_size) / steps + 1][B] = (i % 255) + 1;
	}
	for(; i < 510; i++) {
		colors[(i * palette_size) / steps + 1][R] = (i % 255) + 1;
		colors[(i * palette_size) / steps + 1][G] = (i % 255) + 1;
		colors[(i * palette_size) / steps + 1][B] = 255;
	}
	for(; i < 765; i++) {
		colors[(i * palette_size) / steps + 1][R] = 255;
		colors[(i * palette_size) / steps + 1][G] = 255;
		colors[(i * palette_size) / steps + 1][B] = 255 - ( i % 255 );
	}
	for(; i < 1020; i++) {
		colors[(i * palette_size) / steps + 1][R] = 255;
		colors[(i * palette_size) / steps + 1][G] = 255 - ( i % 255 );
		colors[(i * palette_size) / steps + 1][B] = 1;
	}
	for(; i < 1275; i++) {
		colors[(i * palette_size) / steps + 1][R] = 255 - ( i % 255 );
		colors[(i * palette_size) / steps + 1][G] = 1;
		colors[(i * palette_size) / steps + 1][B] = 1;
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
