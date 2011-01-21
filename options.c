#include <getopt.h>
#include <stdlib.h>

#include "options.h"

#define PROG_NAME "frac"

void usage(FILE *stream) {
	fprintf(stream, "Usage: %s [OPTIONS]\n", PROG_NAME);
}
void help(FILE *stream) {
	usage(stream);
	fprintf(stream, "\tCalculation\n");
	fprintf(stream, "  -I, --max-iter\tMaximum number of iterations\n");
	fprintf(stream, "  -T, --threshold\tThreshold value\n");
	fprintf(stream, "\tReal values\n");
	fprintf(stream, "  -x, --min-x\t\tMinimum value for X\n");
	fprintf(stream, "  -X, --max-x\t\tMaximum value for X\n");
	fprintf(stream, "\tImaginary values\n");
	fprintf(stream, "  -y, --min-y\t\tMinimum value for Y\n");
	fprintf(stream, "  -Y, --max-y\t\tMaximum value for Y\n");
	fprintf(stream, "\tImage information\n");
	fprintf(stream, "      --geometry\tResolution of the image (ie 1280x1024)\n");
	fprintf(stream, "\n");
	fprintf(stream, "  -h, --help\t\tShow this help message\n");
}

void usage_error(const char *message) {
		usage(stderr);
		fprintf(stderr, "\t%s\n",message);
		exit(-1);
}

int iteration_max = 0;
double min_x = 0;

void parse_options(int argc, char *argv[]) {
	static struct option long_options[] = {
		{"help", no_argument, 0, 'H'},
		{"min-x", required_argument, 0, 'x'},
		{"max-x", required_argument, 0, 'X'},
		{"min-y", required_argument, 0, 'y'},
		{"max-y", required_argument, 0, 'Y'},
		{"geometry", required_argument, 0, 'G'},
		{"iterations", required_argument, 0, 'I'},
		{"threshold", required_argument, 0, 'T'},
		{0, 0, 0, 0}
	};


	for(;;) {
		int option_index;
		int c;
		c = getopt_long(argc, argv, "I:T:x:X:y:Y:h", long_options, &option_index);

		/* End of options */
		if(c == -1) {
			break;
		}

		switch(c) {
			case 0:
				/* A flag was already set */
				if(long_options[option_index].flag != 0) {
					break;
				}
				printf("option %s", long_options[option_index].name);
				if(optarg) {
					printf(" with arg %s", optarg);
				}
				printf("\n");
				break;
			case 'h':
				usage(stdout);
				exit(1);
			case 'H':
				help(stdout);
				exit(1);
			case 'I':
				iteration_max = (int)strtol(optarg,NULL,10);
				break;
			case 'T':
				/* Parse int */
				break;
			case 'x':
				min_x = strtod(optarg,NULL);
				break;
			case 'X':
			case 'y':
			case 'Y':
				/* Parse float */
				break;
			case 'G':
				/* Parse string */
				/* resolution_x = strtol(optarg,"x",10); */
				break;
		}
	}

	fprintf(stderr, "x: %lf\n", min_x);
	if(iteration_max <= 0) {
		usage_error("Iterations must be larger than 0");
	}
	else {
		fprintf(stderr, "I: %u\n", iteration_max);
	}
}
