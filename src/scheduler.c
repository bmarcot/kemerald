/**
 * Mandatory includes
 *
 */
#include "types.h"
#include "kernel.h"
#include "scheduler.h"

/**
 * Additional includes
 *
 */
#include "stdio.h"
#include "stdlib.h"
#include "msr.h"
#include "syscall.h"
#include "mmu.h"
#include "list.h"
#include "thread.h"
#include "utils.h"


void job1(void)
{
  uint32_t esp, ebp;
  int32_t status = 0xbebe;

  while (1)
    {
      esp = rdesp();
      ebp = rdebp();
      printf("\t(job1)\tesp:%x ebp:%x\n", esp, ebp);

      status = sc_sysenter(1);

      //*(uint32_t *)(0xdeadbeef) = 0xbabababa;

      printf("\tsc_sleep() exit code = %d\n", status);
    }
}

void job2(void)
{
  uint32_t esp, ebp;
  int32_t status = 0xbebe;

  while (1)
    {
      esp = rdesp();
      ebp = rdebp();

      printf("\t(job2)\tesp:%x ebp:%x\n", esp, ebp);

      status = sc_sysenter(1);
      printf("\tsc_sleep() exit code = %d\n", status);
    }
}


/**
 * sched_init():
 *
 */
int32_t			sched_init(void)
{
  int32_t		id[2];
  uint32_t		i;

  /* XXX: la création des threads devrait être dans un autre module */

  /* create a first job */
  if ((id[0] = thread_create((uint32_t)(job1), USER, "job1")) == ERR_UNKNOWN)
    {
      return ERR_UNKNOWN;
    }
  printf("Created thread job1 with id %d.\n", id[0]);

  /* create a second job */
  if ((id[1] = thread_create((uint32_t)(job2), USER, "job2")) == ERR_UNKNOWN)
    {
      return ERR_UNKNOWN;
    }
  printf("Created thread job2 with id %d.\n", id[1]);

  /* start all the tasks */
  for (i = 0; i < 2; i++)
    {
      if (thread_start(id[i]) != ERR_NONE)
	{
	  return ERR_UNKNOWN;
	}
    }

  return ERR_NONE;
}


/**
 * sched_elect():
 *
 */
int32_t		sched_elect(void)
{
  list_s	*threads = thread_get_ready();
  
  /* Walk the ready's list to find the running thread. */
  while (((thread_ctx_s *)(threads->e))->state != RUNNING)
    {
      threads = threads->next;
    }


  if (threads->next)
    {
      thread_switch((thread_ctx_s *)(threads->e),
		    (thread_ctx_s *)(threads->next->e));
    }
  else
    {
      thread_switch((thread_ctx_s *)(threads->e),
		    (thread_ctx_s *)(thread_get_ready()->e));
    }

  return ERR_NONE;
}


/**
 * sched_launch():
 *
 */
void		sched_launch(void)
{
  /* Launch the scheduling. */
  thread_launch();
}
