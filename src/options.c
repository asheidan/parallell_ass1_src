#include <stdlib.h>

#include <getopt.h>

#include "options.h"

unsigned int iteration_max = 1000;
unsigned int threshold = 2;

unsigned int res_x = 600;
unsigned int res_y = 400;

coord_t min_x = -2.0;
coord_t max_x =  1.0;

coord_t min_y = -1.0;
coord_t max_y =  1.0;

unsigned int palette_size = 300;

bool output_to_terminal = false;
char *filename = "output.ppm";
bool save_image = true;

bool continous_smoothing = false;
bool print_time = false;
unsigned int num_runs = 1;
bool show_magic = false;

unsigned int verbosity = 0;

bool pthreads = false;
bool openmp = false;

unsigned int num_threads = 2;

unsigned int magic_size = 0;
unsigned int idle_thread_wait = 100;

void option_error(FILE *stream) {
	fprintf(stream,"Try `%s --help` for more information.\n",PROG_NAME);
}

void usage(FILE *stream) {
	fprintf(stream, "Usage: %s [OPTIONS]\n", PROG_NAME);
}

void help(FILE *stream) {
	usage(stream);
	fprintf(stream, "\n");

	fprintf(stream, "\tReal values\n");
	fprintf(stream, "  -x, --min-x=REAL\t\tMinimum value for X\n");
	fprintf(stream, "  -X, --max-x=REAL\t\tMaximum value for X\n");
	fprintf(stream, "  -r, --center-x=REAL\t\tCenter value for X\n");
	fprintf(stream, "  -w, --horizontal-diameter=DISTANCE\n\t\t\t\tValue intervall for X\n");
	fprintf(stream, "\n");

	fprintf(stream, "\tImaginary values\n");
	fprintf(stream, "  -y, --min-y=IMAGINARY\t\tMinimum value for Y\n");
	fprintf(stream, "  -Y, --max-y=IMAGINARY\t\tMaximum value for Y\n");
	fprintf(stream, "  -i, --center-y=IMAGINARY\tCenter value for Y\n");
	fprintf(stream, "  -h, --vertical-diameter=DISTANCE\n\t\t\t\tValue intervall for Y\n");
	fprintf(stream, "\n");

	fprintf(stream, "\tImage information\n");
	fprintf(stream, "  -G, --geometry=WIDTHxHEIGHT\tResolution of the image (ie 1280x1024)\n");
	fprintf(stream, "  -p, --palette=COLORS\t\tNumber of colors in palette\n");
	fprintf(stream, "  -S, --smoothing\t\tContinous smoothing of colors\n");
	fprintf(stream, "\n");

	fprintf(stream, "\tCalculation\n");
	fprintf(stream, "      --threads[=NUMBER]\tUse NUMBER threads to calculate image\n");
	fprintf(stream, "  -O, --openmp[=THREADS]\tUse OpenMP threads\n");
	fprintf(stream, "  -P, --pthread[=THREADS]\tUse PThreads threads\n");
	fprintf(stream, "  -M, --magic[=MINSIZE]\t\tUse magic boxes width minimum size of MINSIZE\n");
	fprintf(stream, "      --idle-wait[=UTIME]\tMake idle threads wait UTIME before requesting more work\n");
	fprintf(stream, "  -I, --max-iter=ITERATIONS\tMaximum number of iterations\n");
	fprintf(stream, "  -T, --threshold=VALUE\t\tThreshold value\n");
	fprintf(stream, "  -t, --time\t\t\tPrint execution time on stdout\n");
	fprintf(stream, "\n");

	fprintf(stream, "  -F, --file=FILENAME\t\tThreshold value\n");
	fprintf(stream, "  -c\t\t\t\tWrite image to stdout\n");
	fprintf(stream, "  -d\t\t\t\tDon't save image\n");
	fprintf(stream, "\n");

	fprintf(stream, "  -n [EXECUTIONS]\t\tRun several times (for higher confidence in timings)\n");
	fprintf(stream, "      --show-magic\t\tShow magic boxes in rendering\n");
	fprintf(stream, "  -v, --verbose[=LEVEL]\t\tIncrease or set verbosity level\n");
	fprintf(stream, "      --help\t\t\tShow this help message\n");
}

void usage_error(const char *message) {
		usage(stderr);
		fprintf(stderr, "\t%s\n",message);
		exit(-1);
}

void parse_geometry(const char *geometry) {
	char *p;
	res_x = (int)strtol(geometry,&p,10);
	if(p!= '\0') p++;
	res_y = (int)strtol(p,NULL,10);
}

void parse_options(int argc, char *argv[]) {
	coord_t
		center_x = 0, center_y = 0,
		vertical_diameter = 0,
		horizontal_diameter = 0;
	static struct option long_options[] = {
		{"help", no_argument, 0, 'H'},
		{"min-x", required_argument, 0, 'x'},
		{"max-x", required_argument, 0, 'X'},
		{"min-y", required_argument, 0, 'y'},
		{"max-y", required_argument, 0, 'Y'},

		{"center-x", required_argument, 0, 'r'},
		{"center-y", required_argument, 0, 'i'},
		{"real-diameter", required_argument, 0, 'w'},
		{"imaginary-diameter", required_argument, 0, 'h'},

		{"palette", required_argument, 0, 'p'},
		{"smoothing", no_argument, 0, 'S'},
		{"geometry", required_argument, 0, 'G'},
		{"iterations", required_argument, 0, 'I'},
		{"threshold", required_argument, 0, 'T'},

		{"threads", required_argument, 0, '|'},
		{"pthreads", no_argument, 0, 'P'},
		{"openmp", no_argument, 0, 'O'},
		{"magic", optional_argument, 0, 'M'},
		{"idle-wait", required_argument, 0, 'W'},

		{"file", required_argument, 0, 'F'},
		{"time", required_argument, 0, 't'},
		{"verbose", optional_argument, 0, 'v'},
		{"show-magic", no_argument, (int*)&show_magic, 1},
		{0, 0, 0, 0}
	};


	for(;;) {
		int option_index;
		int c;
		c = getopt_long(argc, argv, "cdSDtvF:I:T:p:x:X:y:Y:r:i:G:h:w:POM:n:", long_options, &option_index);

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
			/*case 'h':*/
				/*usage(stdout);*/
				/*exit(1);*/
			case 'c':
				output_to_terminal = true;
				break;
			case 'd':
				save_image = false;
				break;
			case 'n':
				num_runs = (unsigned int)strtoul(optarg,NULL,0);
				break;
			case 't':
				print_time = true;
				break;
			case 'v':
				if(optarg == NULL) {
					verbosity++;
				}
				else {
					verbosity = (unsigned int)strtoul(optarg,NULL,0);
				}
				break;
			case 'F':
				filename = optarg;
				break;
			case 'H':
				help(stdout);
				exit(1);
			case 'I':
				iteration_max = (unsigned int)strtoul(optarg,NULL,0);
				break;
			case 'T':
				/* Parse int */
				threshold = (unsigned int)strtoul(optarg,NULL,0);
				break;
			case 'p':
				palette_size = (unsigned int)strtoul(optarg,NULL,0);
				break;
			case 'S':
				continous_smoothing = true;
				break;
			case 'x':
				min_x = strtold(optarg,NULL);
				break;
			case 'X':
				max_x = strtold(optarg,NULL);
				break;
			case 'y':
				min_y = strtold(optarg,NULL);
				break;
			case 'Y':
				max_y = strtold(optarg,NULL);
				break;
			case 'i':
				center_y = strtold(optarg,NULL);
				break;
			case 'r':
				center_x = strtold(optarg,NULL);
				break;
			case 'w':
				horizontal_diameter = strtold(optarg,NULL);
				break;
			case 'h':
				vertical_diameter = strtold(optarg,NULL);
				break;
			case 'G':
				/* Parse string */
				parse_geometry(optarg);
				break;
			case '|':
				num_threads = strtoul(optarg,NULL,0);
				break;
			case 'P':
				openmp = false;
				pthreads = true;
				break;
			case 'O':
				pthreads = false;
				openmp = true;
				break;
			case 'M':
				openmp = false;
				pthreads = false;
				if(optarg == NULL) {
					magic_size = 256;
				}
				else {
					magic_size = (unsigned int)strtoul(optarg, NULL, 0);
				}
				break;
			case 'W':
				idle_thread_wait = (unsigned int)strtoul(optarg, NULL, 0);
				break;
			case ':':
			case '?':
			default:
				option_error(stderr);
				exit(-1);
		}

	}

	if(horizontal_diameter > 0) {
		min_x = center_x - (horizontal_diameter / 2.0);
		max_x = center_x + (horizontal_diameter / 2.0);
	}
	if(vertical_diameter > 0) {
		min_y = center_y - (vertical_diameter / 2.0);
		max_y = center_y + (vertical_diameter / 2.0);
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

	if(palette_size > 1275) {
		usage_error("The palette-size should not be larger than 1275 color steps");
	}
}
