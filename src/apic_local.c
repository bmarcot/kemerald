/**
 * Mandatory includes
 *
 */
#include "types.h"
#include "kernel.h"
#include "apic_local.h"

/**
 * Additional includes
 *
 */
#include "mmu.h"
#include "msr.h"
#include "utils.h"
#include "interrupts.h"


/**
 * TODO:
 *
 * - set the memory-mapped registers in a Strong Uncacheable zone (UC).
 * - convert _apic_local_base in an array on a MP system because each CPU
 *   must own his own 4kb memory-mapped registers zone.
 */


/* XXX: On a MP systems, this static var shall become a static array. */
static uint32_t		_apic_local_base;


/**
 * apic_local_init():
 *
 * MSR_IA32_APIC_BASE_ENABLE
 * Depending on the Local-APIC implementation, clearing this bit might prevent
 * from enabling it again later. This bit should be always set, and system should
 * rather rely on the Spurious Interrupt bit in order to enable/disable the
 * Local-APIC.
 */
int32_t			apic_local_init(void)
{
  uint32_t		msr;

  /* Get from the MSRs the Local-APIC base address for the memory-mapped registers. */
  msr = (uint32_t) rdmsr(MSR_IA32_APIC_BASE);

  /* Get the memory-mapped registers address base. */
  _apic_local_base = msr & ~0xfff;

  /* APIC global should be always set enable in the MSR. */
  msr |= MSR_IA32_APIC_BASE_ENABLE;

  /* Rewrite the updated value into the MSR. */
  wrmsr(MSR_IA32_APIC_BASE, ((uint64_t) msr) & 0xffffffff);

  printf("Before reserve: %x\n", _apic_local_base);

  /* Reserve a page for the Local-APIC memory-mapped registers. */
  /* XXX: ZONE MUST BE STRONG UC ! p. 8-6 */
  if (mmu_virt_page_reserve((vaddr_t) _apic_local_base,
			    MMU_PAGE_OPT_READWRITE
			    /* | MMU_PAGE_OPT_STRONG_UNCACHEABLE */
			    | MMU_PAGE_OPT_WRITETROUGH
			    | MMU_PAGE_OPT_CACHE_DISABLE
			    | MMU_PAGE_OPT_IDENTITY))
    {
      printf("Can't reserve a page for the Local-APIC memory-mapped registers.\n");
      INFINITE_LOOP();
      return ERR_UNKNOWN;
    }

  return ERR_NONE;
}


/**
 * apic_local_enable():
 *
 */
int32_t			apic_local_enable(bool	b)
{
  uint32_t		svr;

  /* Read the Spurious-Interrupt Vector Register from the memory-mapped registers. */
  svr = _apic_local_rdreg(APIC_LOCAL_REG_OFFSET_SVR);

  if (b == true)
    {
      /* Set the enable bit in SVR. */
      svr |= APIC_LOCAL_REG_FLAG_SVR_ENABLE;
    }
  else
    {
      /* Reset the enable bit in SVR. */
      svr &= ~APIC_LOCAL_REG_FLAG_SVR_ENABLE;
    }

  /* Rewrite the updated memory-mapped register. */
  _apic_local_wrreg(APIC_LOCAL_REG_OFFSET_SVR, svr);

  return ERR_NONE;
}


/**
 * apic_local_ack():
 *
 */
void			apic_local_ack(void)
{
  /* Reset the End Of Interrupt register. */
  _apic_local_wrreg(APIC_LOCAL_REG_OFFSET_EOI, 0);
}


/**
 * _apic_local_rdreg():
 *
 */
static uint32_t		_apic_local_rdreg(uint32_t	reg_offset)
{
  /* Return the content of a memory-mapped register. */
  return rdmem32((addr_t)(_apic_local_base + reg_offset));
}


/**
 * _apic_local_wrreg():
 *
 */
static void		_apic_local_wrreg(uint32_t	reg_offset,
					 uint32_t	reg_value)
{
  /* Write a value to a memory-mapped register. */
  wrmem32((addr_t)(_apic_local_base + reg_offset), reg_value);
}


/**
 * _apic_local_get_id():
 *
 */
static uint8_t		_apic_local_get_id(void)
{
  /* Return the Local-APIC id value. */
  return (uint8_t)(_apic_local_rdreg(APIC_LOCAL_REG_OFFSET_ID) >> 24);
}


/**
 * _apic_local_set_id():
 *
 */
static void		_apic_local_set_id(uint8_t	id)
{
  /* Set the Local-APIC id value. */
  _apic_local_wrreg(APIC_LOCAL_REG_OFFSET_ID, (uint32_t) id << 24);
}


/**
 * _apic_local_get_version():
 *
 */
static uint8_t		_apic_local_get_version(void)
{
  /* Return the Local-APIC version value. */
  return (uint8_t) _apic_local_rdreg(APIC_LOCAL_REG_OFFSET_VERSION);
}


/**
 * _apic_local_get_max_lvt_entry():
 *
 */
static uint8_t		_apic_local_get_max_lvt_entry(void)
{
  /* Return the Local-APIC max number of LVT entries. */
  return (uint8_t)((_apic_local_rdreg(APIC_LOCAL_REG_OFFSET_VERSION) >> 16) + 1);
}


/**
 * apic_local_timer_config():
 *
 */
int32_t			apic_local_timer_config(uint32_t	initial_count)
{
  uint32_t		lvt_timer;

  /* Configure the Divide Configuration Register. */
  _apic_local_wrreg(APIC_LOCAL_REG_OFFSET_DIVIDE_CONFIG, APIC_LOCAL_DIVIDE_CONFIG_16X);

  /* Configure the Intial-Count register. */
  _apic_local_wrreg(APIC_LOCAL_REG_OFFSET_INIT_COUNT, initial_count);

  /* Configure the Current-Count register. */
  _apic_local_wrreg(APIC_LOCAL_REG_OFFSET_CUR_COUNT, initial_count);

  /* Read the initial LVT timer register value. */
  lvt_timer = _apic_local_rdreg(APIC_LOCAL_REG_OFFSET_LVT_TIMER);

  /* Configure the timer as periodic. */
  lvt_timer |= APIC_LOCAL_REG_FLAG_LVT_TIMER_PERIODIC;

  /* Set the IDT vector's index. */
  lvt_timer = (lvt_timer & ~0xff) | INT_APIC_LOCAL_TIMER;

  /* Unmask the LVT entry to allow the timer interrupt triggering. */
  lvt_timer &= ~APIC_LOCAL_REG_FLAG_LVT_MASKED;

  /* Write the updated entry to the LVT. */ 
  _apic_local_wrreg(APIC_LOCAL_REG_OFFSET_LVT_TIMER, lvt_timer);

  return ERR_NONE;
}


/**
 * apic_local_set_task_priority():
 *
 */
int32_t			apic_local_set_task_priority(uint8_t	task_priority)
{
  /* Check if the task_priority is valid. */
  if (task_priority > 15)
    {
      return ERR_UNKNOWN;
    }

  /* Set the requested level of task_priority to trigger an interrupt. */
  _apic_local_wrreg(APIC_LOCAL_REG_OFFSET_TASK_PRIORITY,
		    (uint32_t) task_priority << 4);

  return ERR_NONE;
}


/**
 * apic_local_get_task_priority():
 *
 */
uint8_t			apic_local_get_task_priority(void)
{
  /* Return the current level of priority of the processor. */
  return (uint8_t)((_apic_local_rdreg(APIC_LOCAL_REG_OFFSET_TASK_PRIORITY) >> 4) & 0xf);
}


/**
 * apic_local_get_proc_priority():
 *
 */
uint8_t			apic_local_get_proc_priority(void)
{
  /* Return the current level of priority of the processor. */
  return (uint8_t)((_apic_local_rdreg(APIC_LOCAL_REG_OFFSET_PROC_PRIORITY) >> 4) & 0xf);
}


/**
 * apic_local_is_pending():
 *
 */
uint8_t			apic_local_is_pending(void)
{
  uint8_t		i = 8;
  uint32_t		irr;
  int8_t		msb;

  /* While no Interrupt Request found, walk the IRR bitmap. */
  while (i >= 0)
    {
      /* Read the higher priority Interrupt Request from the IRR register. */
      irr = _apic_local_rdreg(APIC_LOCAL_REG_OFFSET_IRR + i * sizeof (uint32_t));

      /* Find the most significant bit set in the IRR. */
      msb = bsr(irr);

      /* If a bit is set, return the bit index (which is the priority). */
      if (msb > 0)
	{
	  return i * 32 + msb;
	}

      /* Read the next register from IRR, but the lower register is reserved. */
      i--;
    }

  return ERR_NONE;
}
