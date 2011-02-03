#include "magic_render.h"

#include <stdlib.h>

#include "options.h"
#include "pthread_common.h"

#include "ILHeap.h"

ILHeap *queue;

void magic_queue_add(int x_start, int x_end, int y_start, int y_end, int priority) {
	worker_task_t
		*task;

	task = malloc(sizeof(worker_task_t));

	task->x_start = x_start;
	task->x_end = x_end;
	task->y_start = y_start;
	task->y_end = y_end;

	pthread_mutex_lock( &queue_lock );
	ILHeapInsert(queue, priority, (void*)task);
	pthread_mutex_unlock( &queue_lock);
}

worker_task_t *magic_queue_get() {
	worker_task_t
		*task;
	pthread_mutex_lock( &queue_lock );
	task = (worker_task_t *)ILHeapRemove(queue);
	pthread_mutex_unlock( &queue_lock );
	return task;
}

void magic_worker(void *a) {
	worker_task_t
		*t;

	t = magic_queue_get();
	if(t == NULL) {
		/* TODO: What if the calculation isn't done but the queue is empty? */
		return;
	}
	else {
		//magic_render(t->x_start, t->x_end, t->y_start, t->y_end);
		free(t);
	}
}

void magic_render(
		unsigned int *image, coord_t step_x, coord_t step_y,
		int x_start, int x_end, int y_start, int y_end) {
	int
		x_size, y_size;

	x_size = x_end - x_start;
	y_size = y_end - y_start;

	queue = ILHeapCreate();

	if(magic_size < (x_size * y_size)) {
		x_size /= 2;
		y_size /= 2;

		/* Add squares to work queue */
		magic_queue_add(x_start,x_start + x_size, y_start, y_start + y_size, x_size * y_size);
		magic_queue_add(x_start + x_size, x_end, y_start, y_start + y_size, x_size * y_size);
		magic_queue_add(x_start,x_start + x_size, y_start + y_size, y_end, x_size * y_size);
		magic_queue_add(x_start + x_size, x_end, y_start + y_size, y_end, x_size * y_size);
	}

	/* Create threads */
	/* Join threads */
	ILHeapFree(queue);
}
