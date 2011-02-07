#ifndef __RENDER_H__
#define __RENDER_H__

#include "coord.h"
#include "pthread_common.h"

pixel_t iterate(coord_t real, coord_t imag);
void render(image_info_t *info, worker_task_t *t);

#endif
