/**
 * Mandatory includes
 *
 */
#include "types.h"
#include "kernel.h"
#include "syscall.h"

/**
 * Additional includes
 *
 */
#include "scheduler.h"
#include "msr.h"
#include "utils.h"
#include "segment.h"

/* to be removed in release candidate */
#include "stdio.h"

/**
 * TODO
 *
 * - use syscall/sysret instead of sysenter/sysexit: 64 bits compliant and restore of EFLAGS.
 */

static int32_t  (*_service_call[])(uint32_t) =
{
  sc_thread_stop,
  sc_sleep
};


/**
 * sc_init():
 *
 */
int32_t         sc_init(void)
{
  /* Configure the CS and EIP sysenter hook. */
  wrmsr(MSR_IA32_SYSENTER_CS, SEG_SELECTOR_RING0_CODE);
  wrmsr(MSR_IA32_SYSENTER_EIP, (uint64_t) sc_hook & 0xffffffff);

  printf("System-call hook set at @: %x\n", (uint32_t)(sc_hook));

  return ERR_NONE;
}


/**
 * sc_sysenter():
 *
 * This call is done from user-side (ring3 threads).
 */
int32_t         sc_sysenter(uint32_t    service_id)
{
  int32_t       status = ERR_UNKNOWN;

  SC_SYSENTER(service_id, &status);

  return status;
}


/**
 * sc_enter():
 *
 * When a syscall is done from ring3, it is hooked in the system at this location.
 */
int32_t         sc_enter(uint32_t       service_id,
                         uint32_t       esp3,
                         uint32_t       eip3)
{
/*   printf("System-call!\n"); */
/*   printf("\tservice id: %d\n", service_id); */
/*   printf("\tESP3:\t%x\n", esp3); */
/*   printf("\tEIP3:\t%x\n", eip3); */

  /* Call the requested service. */
  return _service_call[service_id](1);
}


/* ------------------- SERVICES ------------------- */

/* XXX: à remplacer par sc_yield() */
int32_t         sc_sleep(uint32_t       delay)
{
  /* activate interrupts to be awaken on the PIT interrupt */
  sti();

  /* put the system in halt mode */
  hlt();

  return ERR_NONE;
}

/**
 * sc_yield():
 *
 */


int32_t         sc_thread_stop(uint32_t thread_id)
{
  printf("Stop thread: %d\n", thread_id);

  return ERR_NONE;
}
