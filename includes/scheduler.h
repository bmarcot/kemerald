#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "thread.h"

int32_t		sched_init(void);
int32_t		sched_elect(void);
int32_t		sched_switch(thread_ctx_s *, thread_ctx_s *);
void		sched_launch(void);

#endif /* !__SCHEDULER_H__ */
