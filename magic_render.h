#ifndef __MAGIC_RENDER_H__
#define __MAGIC_RENDER_H__

#include "coord.h"
#include "pthread_common.h"

void magic_openmp_render(image_info_t *info, worker_task_t *task);
void magic_render(image_info_t *info, worker_task_t *task);

#endif
