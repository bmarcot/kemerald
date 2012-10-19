/**
 * Mandatory includes
 *
 */
#include "types.h"
#include "kernel.h"
#include "isr_stage2.h"

/**
 * Additional includes
 *
 */
#include "idt2.h"
#include "utils.h"
#include "apic_local.h"

/* XXX: a enlever en release... */
#include "console.h"

uint32_t		__isr_stage2[IDT_N];
//uint32_t		__isr_stage2_int[IDT_N - IDT-]


/**
 * isr_stage2_init():
 *
 */
int32_t			isr_stage2_init(void)
{
  uint32_t		i;

  for (i = 0; i < IDT_N; i++)
    {
      __isr_stage2[i] = (uint32_t) _isr_stage2_unhandled;
    }

  return ERR_NONE;
}


/**
 * isr_stage2_bind():
 *
 */
void			isr_stage2_bind(uint32_t	f,
					uint8_t		index)
{
  __isr_stage2[index] = f;
}


#define APIC_LOCAL_USER_DEFINED_INT	32
typedef void (*func)(uint32_t);
/**
 * isr_stage2_pending():
 *
 */
void			isr_stage2_pending(void)
{
  uint8_t		pending_n;

  apic_local_ack();
  printf("Int acked.\n");

/*   /\* Probe the Local-APIC for pending interrupts. *\/ */
/*   pending_n = apic_local_is_pending(); */

/*   /\* If an interrupt is actually pending, and if its priority is greater than */
/*      the current processor level, then handle it. *\/ */
/*   if ((pending_n >= APIC_LOCAL_USER_DEFINED_INT) */
/*       && ((pending_n / 16) >= apic_local_get_task_priority())) */
/*     { */
/*       /\* Call the relevant ISR. *\/ */
/*       ((func) __isr_stage2[pending_n])(0); */
/*     } */
}


/**
 * isr_stage2_unhandled():
 *
 */
static void		_isr_stage2_unhandled(uint32_t	code)
{
  console_printf("#Unhandled Interrupt\n", BG_RED | FG_WHITE | FG_INTENSITY);
  printf("#Unhandled Interrupt (%d)\n", code);

  /* Halt the system. */
  while (1)
    hlt();
}
