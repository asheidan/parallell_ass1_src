#include "pthread_render.h"
#include "pthread_common.h"

#include <stdlib.h>
#include <pthread.h>

#include "options.h"
#include "render.h"

#include <stdio.h>

int progress;

void *work_thread(void *t) {
	int
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

void pthread_render(
		unsigned int *image, coord_t step_x, coord_t step_y,
		int x_start, int x_end, int y_start, int y_end) {
	pthread_t
		*worker_threads;
	pthread_attr_t
		attr;
	unsigned long
		i;
	worker_argument_t
		argument;
	worker_task_t
		task;
	image_info_t
		info;

	argument.task = &task;
	argument.info = &info;

	info.step_x = step_x;
	info.step_y = step_y;
	info.buffer = image;

	task.x_start = x_start;
	task.x_end = x_end;
	task.y_start = y_start;
	task.y_end = y_end;

	worker_threads = malloc(sizeof(pthread_t) * ( pthreads - 1 ));
	if(worker_threads == NULL) {
		perror(PROG_NAME);
		exit(3);
	}

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	progress = y_start;
	
	/* Creating threads */
	for(i = 0; i < (num_threads - 1); i++) {
		pthread_create(&worker_threads[i], &attr, work_thread, (void *)&argument);
	}

	work_thread((void *)&argument);

	/* Waiting for other threads to die */
	for(i = 0; i < (pthreads - 1); i++) {
		pthread_join(worker_threads[i], NULL);
	}

	free(worker_threads);
}
