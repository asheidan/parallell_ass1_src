#include "timer.h"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

struct timeval *timevalue;

void timer_start() {
	if(timevalue == NULL) {
		timevalue = malloc(sizeof(struct timeval));
		if(timevalue == NULL) {
			perror("TIMER");
			exit(-3);
		}
	}
	gettimeofday(timevalue, NULL);
}

double timer_check() {
	struct timeval
		end;
	double
		result;

	gettimeofday(&end, NULL);

	result = (double)( end.tv_sec - timevalue->tv_sec ) + (double)( end.tv_usec - timevalue->tv_usec ) * 1E-6;
	if(timevalue != NULL) {
		return result;
	}
	else {
		return -1.0;
	}
}

double timer_stop() {
	double result;
	result = timer_check();
	free(timevalue);
	return result;
}
