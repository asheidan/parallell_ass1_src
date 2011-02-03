#ifndef __PTREAD_COMMON_H__
#define __PTREAD_COMMON_H__

#include <pthread.h>

#include "coord.h"

typedef struct {
	int
		x_start, x_end,
		y_start, y_end;
} worker_task_t;

typedef struct {
	unsigned int *buffer;
	coord_t step_x;
	coord_t step_y;
} image_info_t;

typedef struct {
	image_info_t
		*info;
	worker_task_t
		*task;
} worker_argument_t;

extern pthread_mutex_t queue_lock;

#endif
