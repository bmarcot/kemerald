/**
 * Mandatory includes
 *
 */
#include "types.h"
#include "kernel.h"
#include "thread.h"

/**
 * Additional includes
 *
 */
#include "utils.h"
#include "msr.h"
#include "list.h"
#include "stdlib.h"
#include "mmu.h"
#include "serial.h"
#include "string.h"
#include "segment.h"

#include "apic_local.h"


static list_s   *_threads_dormant;
static list_s   *_threads_ready;
static list_s   *_threads_waiting;


/**
 * thread_init():
 *
 */
int32_t         thread_init(void)
{
  int32_t       bg_id;

  /* Initialize the threads lists. */
  _threads_dormant = NULL;
  _threads_ready = NULL;
  _threads_waiting = NULL;

  /* Create the background job. */
  if ((bg_id = thread_create((uint32_t)(thread_bg), SYSTEM, "bg"))
      == ERR_UNKNOWN)
    {
      return ERR_UNKNOWN;
    }

  /* Put the background job into the ready list. */
  if (thread_start(bg_id) != ERR_NONE)
    {
      return ERR_UNKNOWN;
    }

  return ERR_NONE;
}


/**
 * thread_create():
 *
 */
int32_t         thread_create(uint32_t          entry,
                              thread_state_e    class,
                              char*             name)
{
  thread_ctx_s  *thread;
  list_s        *new;
  int32_t       id;

  /* Allocate the thread context into memory. */
  if ((thread = malloc(sizeof (thread_ctx_s))) == NULL)
      return NULL;

  /* Get a unique id for the thread object. */
  if ((id = serial_generate()) == ERR_UNKNOWN)
    {
      return ERR_UNKNOWN;
    }

  /* Set this id to this tread. */
  thread->id = (uint32_t) id;

  /* Set the NORMAL priority. */
  thread->priority = /*NORMAL*/127;

  /* Set the DORMANT state. */
  thread->state = DORMANT;

  /* Configure the thread's class: either USER or SYSTEM. */
  thread->class = class;

  /* Configure the thread entry point. */
  thread->entry = entry;

  /* Allocate a stack page for the Ring0. */
  thread->esp0 = (uint32_t)(malloc(MMU_PAGE_SIZE) + MMU_PAGE_SIZE);

  /* Allocate a stack page for the Ring3. */
  if (class == USER)
    {
      /* XXX: *allocate_user_stack() : ALLOUER DES PAGES alignées et pas de blocs */
      thread->esp3 = (uint32_t)(malloc(MMU_PAGE_SIZE) + MMU_PAGE_SIZE);
    }
  else
    {
      thread->esp3 = NULL;
    }

  /* Configure the task TSS. */
  thread->tss.esp0 = thread->esp0;
  thread->tss.iomap = 0;

  /* XXX: to be removed... */
  memzero(thread->name, 8);
  if (name)
    {
      strcpy(thread->name, name);
    }

  /* Create the list object for this thread. */
  if ((new = list_new(thread)) == NULL)
    {
      return ERR_UNKNOWN;
    }

  /* Insert the new thread into the dormant's thread list. */
  if (list_insert(&_threads_dormant, new) != ERR_NONE)
    {
      return ERR_UNKNOWN;
    }

  return thread->id;
}


/**
 * thread_start():
 *
 */
int32_t         thread_start(uint32_t           tostart_id)
{
  list_s        *it = _threads_dormant;

  /* Walk the dormant's list. */
  while (it)
    {
      if (((thread_ctx_s *)(it->e))->id == tostart_id)
        {
          /* Remove the thread from the dormant's list. */
          if (list_remove(&_threads_dormant, it) != ERR_NONE)
            {
              return ERR_UNKNOWN;
            }

          /* Insert the thread into the ready's list. */
          if (list_insert(&_threads_ready, it) != ERR_NONE)
            {
              return ERR_UNKNOWN;
            }

          return ERR_NONE;
        }
      it = it->next;
    }

  return ERR_UNKNOWN;
}


/**
 * thread_stop():
 *
 */
int32_t         thread_stop(uint32_t            tostop_id)
{
  /* XXX: rajouter un parcours des threads_waiting aussi */
  list_s        *it = _threads_ready;

  /* Walk the ready's list. */
  while (it)
    {
      if (((thread_ctx_s *)(it->e))->id == tostop_id)
        {
          /* Remove the thread from the ready's list. */
          if (list_remove(&_threads_ready, it) != ERR_NONE)
            {
              return ERR_UNKNOWN;
            }

          /* Set the thread's state to DORMANT. */
          ((thread_ctx_s *)(it->e))->state = DORMANT;

          /* Insert the thread into the dormant's list. */
          if (list_insert(&_threads_dormant, it) != ERR_NONE)
            {
              return ERR_UNKNOWN;
            }

          return ERR_NONE;
        }
      it = it->next;
    }

  return ERR_UNKNOWN;
}


/**
 * thread_switch():
 *
 */
int32_t         thread_switch(thread_ctx_s      *prev,
                              thread_ctx_s      *next)
{
  /* Argument checking. */
  if (next == NULL)
    {
      return ERR_NULLPTR;
    }

  /* Reconfigure the TSS with the next thread's esp0. */
  seg_tss_set_esp0(next->tss.esp0);

  /* For sysenter use, and fast syscalls. */
  wrmsr(MSR_IA32_SYSENTER_ESP, (uint64_t) next->tss.esp0 & MASK_32BITS);

  if (next->state == DORMANT)
    {
      /* The next thread becomes RUNNING. */
      next->state = RUNNING;

      if (prev != NULL)
        {
          prev->state = READY;

          /* Save ebp and eflags of the previous thread. */
          THREAD_SETUP_STACK(prev->esp0);
        }

      /**
       * XXX:
       *
       */
      apic_local_ack();

      // class à remplacer par privilege ?
      if (next->class == USER)
        {
          /* If the next thread is a user-level thread, starts in Ring3. */
          THREAD_START_IN_RING3(next->entry, next->esp3);
        }
      else
        {
          /* Else, the next thread is a kernel-level thread, starts in Ring0. */
          THREAD_START_IN_RING0(next->entry, next->esp0);
        }
    }
  else
    {
      /* Set the previous thread's state. */
      if ((prev != NULL) && (prev->state != DORMANT))
        {
          prev->state = READY;
        }

      /* The next thread becomes RUNNING. */
      next->state = RUNNING;

      /* Make the thread swicth from prev to next. */
      THREAD_SWITCH(prev->esp0, next->esp0);
    }


  return ERR_NONE;
}


/**
 * thread_launch():
 *
 */
void            thread_launch(void)
{
  /* Launch the background job. */
  thread_switch(NULL, (thread_ctx_s *)(_threads_ready->e));
}


/**
 * thread_get_ready():
 *
 */
list_s          *thread_get_ready(void)
{
  /* Return a pointer to the ready's list. */
  return _threads_ready;
}


/**
 * thread_bg():
 *
 */
void            thread_bg(void)
{
  while (true)
    {
      printf("\t(bg)\tesp:%x ebp:%x - going to sleep state...\n", rdesp(), rdebp());
      hlt();
    }
}
