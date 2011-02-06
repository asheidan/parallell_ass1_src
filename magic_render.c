#include "magic_render.h"

#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

#include "options.h"
#include "pthread_common.h"
#include "render.h"

#include "ILHeap.h"

ILHeap *queue;
bool *worker_status;

bool work_done() {
	bool result = true;
	int i;
	for(i = 0; result && i < num_threads; i++) {
		result = result && worker_status[i];
	}

	return result;
}

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

bool magic_split(worker_task_t *t) {
	int
		x_size, y_size,
		x_start, x_end,
		y_start, y_end;

	x_start = t->x_start;
	x_end = t->x_end;
	y_start = t->y_start;
	y_end = t->y_end;

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
		return true;
	}
	else {
		return false;
	}
}

bool magic_outline(image_info_t *info,worker_task_t *task) {
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

	if(*(current = &image[res_x * y + x]) == 0) {
		ci = max_y - step_y * y;
		cr = min_x + step_x * x;
		*current = iterate(cr,ci);
		/*fprintf(stderr,"*");*/
	}
	else {
		/*fprintf(stderr," ");*/
	}

	previous = *current;
	/*fprintf(stderr,"[%3d,%3d][ %d]\n",x,y,*current);*/

	for(y = task->y_start; y < task->y_end; y += (task->y_end - task->y_start - 1)) {
		for(x = task->x_start + 1; x < task->x_end; x++) {
			current = &image[res_x * y + x];
			if(*current == 0) {
				ci = max_y - step_y * y;
				cr = min_x + step_x * x;
				*current = iterate(cr,ci);
				/*fprintf(stderr,"*");*/
			}
			else {
				/*fprintf(stderr," ");*/
			}
			/*fprintf(stderr,"[%3d,%3d][ %d]\n",x,y,*current);*/
			if(*current != previous) {
				/* stop and split if larger than minimum size*/
				magic_split(task);
				return false;
			}
		}
	}
	for(x = task->x_start; x < task->x_end; x += (task->x_end - task->x_start - 1)) {
		for(y = task->y_start + 1; y < task->y_end; y++) {
			current = &image[res_x * y + x];
			if(*current == 0) {
				ci = max_y - step_y * y;
				cr = min_x + step_x * x;
				*current = iterate(cr,ci);
				/*fprintf(stderr,"*");*/
			}
			else {
				/*fprintf(stderr," ");*/
			}
			/*fprintf(stderr,"[%3d,%3d][ %d]\n",x,y,*current);*/
			if(*current != previous) {
				/* stop and split if larger than minimum size*/
				magic_split(task);
				return false;
			}
		}
	}
	/*fprintf(stderr, "OPTIMIZZZED---------------------------------\n");*/
	if(show_magic) {
		/*previous = (previous + palette_size / 20) % iteration_max;*/
		if(previous < iteration_max) {
			previous = (previous + palette_size / 32) % iteration_max;
		}
		else {
			previous = palette_size / 2 - palette_size / 10;
		}
	}
	for(y = task->y_start + 1; y < (task->y_end - 1); y++) {
		/*current = image + (res_y * y);*/
		/*current++;*/
		for(x = task->x_start + 1; x < (task->x_end - 1); x++) {
			current = &image[res_x * y + x];
			*current = previous;
			/* *current = (task->x_end * task->y_end) % iteration_max;*/
			/**current = 250;*/
		}
	}
	return true;
}

void magic_worker(void *a) {
	worker_task_t
		*t;
	image_info_t
		*info;
	
	info = (image_info_t *)a;

	while(!work_done()) {
		while((t  = magic_queue_get()) != NULL) {
			worker_status[omp_get_thread_num()] = false;
			/*
			fprintf(stderr,
					"\n\tTask: {(%3d,%3d),(%3d,%3d)}   {%4d,%4d}\n",
					t->x_start, t->y_start, t->x_end, t->y_end,
					 (t->x_end-t->x_start), (t->y_end-t->y_start) 
				);
				*/
			if(magic_size < (t->x_end - t->x_start)*(t->y_end - t->y_start)) {
				magic_outline(info, t);
			}
			else {
				render(info,t);
			}
			free(t);
		}
		worker_status[omp_get_thread_num()] = true;
		if(t == NULL) {
			usleep((useconds_t)idle_thread_wait);
		}
	}
	/* TODO: What if the calculation isn't done but the queue is empty? */
	/* Check status array, if all other threads are idle, we're done */
	return;
}

void magic_render(image_info_t *info, worker_task_t *task) {
	magic_openmp_render(info, task);
}
void magic_openmp_render(image_info_t *info, worker_task_t *task) {
	int
		i;

	queue = ILHeapCreate();

	worker_status = malloc(sizeof(bool) * num_threads);
	if(worker_status == NULL) {
		perror("frac");
		if(verbosity > 3) fprintf(stderr, "worker_status\n");
		exit(3);
	}
	for(i = 0;i < num_threads; i++) {
		worker_status[i] = false;
	}


	/* TODO: Split until there is work for all threads */
	if(!magic_split(task)) {
		render(info,task);
	}
	else {

	omp_set_num_threads(num_threads);

	/* Create threads */
#pragma omp parallel
		{

			magic_worker((void*)info);

		}
	/* Join threads */

	}
	ILHeapFree(queue);
	free(worker_status);
}
