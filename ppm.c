#include "ppm.h"
#include <stdio.h>

#include "options.h"


void ppm_write(int width, int height , int *buffer, int max_count) {
	int buffer_size,i, tmp;
	buffer_size = width * height;

	fprintf(stdout, "P6\n");
	fprintf(stdout, "%d %d\n%d\n", width, height, 255);

	for(i = 0; i < (3 * buffer_size); i++) {
		tmp = buffer[i/3];
		/*fprintf(stderr, "D: 0x%02x\n", buffer[i/3] % 256);*/
		if(tmp == iteration_max) {
			fputc(0,stdout);
		}
		else {
			fputc(tmp % 255, stdout);
		}
	}
}
