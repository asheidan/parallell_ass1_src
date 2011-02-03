#include "magic_render.h"

#include <stdlib.h>

#include "options.h"
#include "pthread_common.h"
#include "render.h"

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

void magic_split(int x_start, int x_end, int y_start, int y_end) {
	int
		x_size, y_size;

	x_size = x_end - x_start;
	y_size = y_end - y_start;

	if(magic_size < (x_size * y_size)) {
		x_size /= 2;
		y_size /= 2;

		/* Add squares to work queue */
		magic_queue_add(x_start,x_start + x_size, y_start, y_start + y_size, x_size * y_size);
		magic_queue_add(x_start + x_size, x_end, y_start, y_start + y_size, x_size * y_size);
		magic_queue_add(x_start,x_start + x_size, y_start + y_size, y_end, x_size * y_size);
		magic_queue_add(x_start + x_size, x_end, y_start + y_size, y_end, x_size * y_size);
	}
}

void magic_outline(image_info_t *info,worker_task_t *task) {
	pixel_t
		previous, *current,
		*image;
	int
		x, y;
	coord_t
		cr, ci,
		step_x, step_y;

	step_x = info->step_x;
	step_y = info->step_y;

	image = info->buffer;

	x = task->x_start;
	y = task->y_start;

	if((previous = image[res_x * y + x]) == 0) {
		previous = iterate(x,y);
	}

	for(; x < task->x_end; x++) {
		current = &image[res_x * y + x];
		if(*current == 0) {
			ci = max_y - step_y * y;
			cr = min_x + step_x * x;
			*current = iterate(cr,ci);
		}
		if(*current != previous) {
			/* stop and split if larger than minimum size*/
		}
	}
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
		pixel_t *image, coord_t step_x, coord_t step_y,
		int x_start, int x_end, int y_start, int y_end) {

	queue = ILHeapCreate();

	magic_split(x_start,x_end,y_start,y_end);

	/* Create threads */
	/* Join threads */
	ILHeapFree(queue);
}
