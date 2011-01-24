#include <stdlib.h>
#include <getopt.h>

#include "options.h"

void option_error(FILE *stream) {
	fprintf(stream,"Try `%s --help` for more information.\n",PROG_NAME);
}

void usage(FILE *stream) {
	fprintf(stream, "Usage: %s [OPTIONS]\n", PROG_NAME);
}

void help(FILE *stream) {
	usage(stream);
	fprintf(stream, "\tCalculation\n");
	fprintf(stream, "  -I, --max-iter=ITERATIONS\tMaximum number of iterations\n");
	fprintf(stream, "  -T, --threshold=VALUE\tThreshold value\n");
	fprintf(stream, "\tReal values\n");
	fprintf(stream, "  -x, --min-x=REAL\tMinimum value for X\n");
	fprintf(stream, "  -X, --max-x=REAL\tMaximum value for X\n");
	fprintf(stream, "\tImaginary values\n");
	fprintf(stream, "  -y, --min-y=IMAGINARY\tMinimum value for Y\n");
	fprintf(stream, "  -Y, --max-y=IMAGINARY\tMaximum value for Y\n");
	fprintf(stream, "\tImage information\n");
	fprintf(stream, "      --geometry=WIDTHxHEIGHT\tResolution of the image (ie 1280x1024)\n");
	fprintf(stream, "  -P, --palette=COLORS\t\tNumber of colors in palette\n");
	fprintf(stream, "\n");
	fprintf(stream, "  -h, --help\t\tShow this help message\n");
}

void usage_error(const char *message) {
		usage(stderr);
		fprintf(stderr, "\t%s\n",message);
		exit(-1);
}

int iteration_max = 1000;
int threshold = 2;

int res_x = 600;
int res_y = 400;

double min_x = -2.0;
double max_x =  1.0;

double min_y = -1.0;
double max_y =  1.0;

int palette_size = 255;

void parse_geometry(const char *geometry) {
	char *p;
	res_x = (int)strtol(geometry,&p,10);
	if(p!= '\0') p++;
	res_y = (int)strtol(p,NULL,10);
}

void parse_options(int argc, char *argv[]) {
	static struct option long_options[] = {
		{"help", no_argument, 0, 'H'},
		{"min-x", required_argument, 0, 'x'},
		{"max-x", required_argument, 0, 'X'},
		{"min-y", required_argument, 0, 'y'},
		{"max-y", required_argument, 0, 'Y'},
		{"geometry", required_argument, 0, 'G'},
		{"iterations", required_argument, 0, 'I'},
		{"palette", required_argument, 0, 'P'},
		{"threshold", required_argument, 0, 'T'},
		{0, 0, 0, 0}
	};


	for(;;) {
		int option_index;
		int c;
		c = getopt_long(argc, argv, "I:T:P:x:X:y:Y:G:h", long_options, &option_index);

		/* End of options */
		if(c == -1) {
			break;
		}

		switch(c) {
			case '?':
			case ':':
				option_error(stderr);
				exit(-1);
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
				iteration_max = (int)strtoul(optarg,NULL,10);
				break;
			case 'T':
				/* Parse int */
				threshold = (int)strtoul(optarg,NULL,10);
				break;
			case 'P':
				palette_size = (int)strtoul(optarg,NULL,10);
				break;
			case 'x':
				min_x = strtod(optarg,NULL);
				break;
			case 'X':
				max_x = strtod(optarg,NULL);
				break;
			case 'y':
				min_y = strtod(optarg,NULL);
				break;
			case 'Y':
				max_y = strtod(optarg,NULL);
				break;
			case 'G':
				/* Parse string */
				parse_geometry(optarg);
				break;
		}

	}

	// TODO: Check all variables for valid values

	if(min_x >= max_x) {
		usage_error("Upper limit of X must be larger than the lower limit");
	}
	if(min_y >= max_y) {
		usage_error("Upper limit of Y must be larger than the lower limit");
	}
	if(iteration_max <= 0) {
		usage_error("Iterations must be larger than 0");
	}
	if(threshold <= 0) {
		usage_error("Threshold must be larger than 0");
	}
	if(res_x < 1) {
		usage_error("Image must have a width");
	}
	if(res_y < 1) {
		usage_error("Image must have a height");
	}
}
