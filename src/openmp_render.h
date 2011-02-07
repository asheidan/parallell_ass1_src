#ifndef __OPENMP_RENDER_H__
#define __OPENMP_RENDER_H__

#include "coord.h"
#include "pthread_common.h"

void openmp_render(image_info_t *info, worker_task_t *task);

#endif
