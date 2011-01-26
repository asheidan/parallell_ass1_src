#include "timer.h"
#include <sys/time.h>

struct timeval timevalue;

void timer_start() {
	gettimeofday(&timevalue, NULL);
}

double timer_check() {
	struct timeval
		end;

	gettimeofday(&end, NULL);

	return (double)( end.tv_sec - timevalue.tv_sec ) + (double)( end.tv_usec - timevalue.tv_usec ) * 1E-6;
}
