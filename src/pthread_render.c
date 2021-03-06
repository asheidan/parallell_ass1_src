#include "pthread_render.h"
#include "pthread_common.h"

#include <stdlib.h>
#include <pthread.h>

#include "options.h"
#include "render.h"

#include <stdio.h>

int progress;

void *work_thread(void *t) {
	unsigned int
		x, y;
	coord_t
		cr, ci;
	worker_task_t
		*task;
	image_info_t
		*info;

	task = ((worker_argument_t *)t)->task;
	info = ((worker_argument_t *)t)->info;

	for(;;) {
		/*pthread_mutex_lock( &queue_lock );*/
		/* Check and increment */
		y = progress++;
		/*pthread_mutex_unlock( &queue_lock );*/
		
		if( y >= res_y ) {
			return((void *)t);
		}

		ci = max_y - (info->step_y) * y;
		for(x = task->x_start; x < (task->x_end); x++) {
			cr = min_x + (info->step_x) * x;

			info->buffer[res_x * y + x] = iterate(cr,ci);
			/*info->buffer[res_x * y + x]++;*/

		}
	}
}

void pthread_render(image_info_t *info, worker_task_t *task) {
	pthread_t
		*worker_threads;
	pthread_attr_t
		attr;
	unsigned long
		i;
	worker_argument_t
		argument;

	argument.task = task;
	argument.info = info;

	worker_threads = malloc(sizeof(pthread_t) * ( num_threads - 1 ));
	if(worker_threads == NULL) {
		perror(PROG_NAME);
		exit(3);
	}

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	progress = task->y_start;
	
	/* Creating threads */
	for(i = 0; i < (num_threads - 1); i++) {
		pthread_create(&worker_threads[i], &attr, work_thread, (void *)&argument);
	}

	work_thread((void *)&argument);

	/* Waiting for other threads to die */
	for(i = 0; i < (num_threads - 1); i++) {
		pthread_join(worker_threads[i], NULL);
	}

	free(worker_threads);
}
