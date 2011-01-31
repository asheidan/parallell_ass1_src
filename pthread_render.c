#include "pthread_render.h"

#include <stdlib.h>
#include <pthread.h>

#include "options.h"

#include <stdio.h>

typedef struct pthread_render_status {
	pthread_mutex_t lock;
} pthread_render_status_t;

void *thread_test(void *t) {
	fprintf(stderr, "Thread %lu is alive\n",(unsigned long)t);
	pthread_exit((void *)t);
}

void pthread_render(
		unsigned int *image, coord_t step_x, coord_t step_y,
		int x_start, int x_end, int y_start, int y_end) {
	pthread_t
		*worker_threads;
	pthread_attr_t
		attr;
	unsigned long
		i;

	worker_threads = malloc(sizeof(pthread_t) * ( pthreads - 1 ));
	if(worker_threads == NULL) {
		perror(PROG_NAME);
		exit(3);
	}

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for(i = 0; i < (pthreads - 1); i++) {
		pthread_create(&worker_threads[i], &attr, thread_test, (void *)i);
	}

	free(worker_threads);
}
