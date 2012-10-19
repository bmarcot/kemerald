/**
 * Mandatory includes
 *
 */
#include "types.h"
#include "kernel.h"
#include "apic_io.h"

/**
 * Additional includes
 *
 */
#include "mmu.h"
#include "utils.h"
#include "interrupts.h"

/**
 * Note: Interrupts 0-15 receive active-high interrupt sources, though interrupts
 * 16-23 receive active-low interrupt sources (for ICH6, p. 139).
 */

/**
 * XXX: No need to convert this var to an array because there is only one IO-APIC,
 * even in a MP system. But in the future, IO-APIC might be more than one...
 */
static uint32_t		_apic_io_base;


/**
 * apic_io_init():
 *
 */
int32_t			apic_io_init(void)
{
  /* Set the base address for memory-mapped registers. */
  _apic_io_base = APIC_IO_BASE;

  /* Reserve a page for the Local-APIC memory-mapped registers. */
  /* XXX: ZONE MUST BE STRONG UC ! p. 8-6 */
  if (mmu_virt_page_reserve((vaddr_t) _apic_io_base,
			    MMU_PAGE_OPT_READWRITE
			    | MMU_PAGE_OPT_CACHE_DISABLE
			    | MMU_PAGE_OPT_WRITETROUGH
			    | MMU_PAGE_OPT_IDENTITY))
    {
      printf("Can't reserve a page for the IO-APIC memory-mapped registers.\n");
      INFINITE_LOOP();
      return ERR_UNKNOWN;
    }

  /* Add a redirection entry for the keyboard. */
  _apic_io_add_redentry(APIC_IO_REG_OFFSET_REDTBL_1,
			INT_APIC_IO_KEYBOARD);

  return ERR_NONE;
}


/**
 * _apic_io_rdreg():
 *
 */
static uint32_t		_apic_io_rdreg(uint32_t	reg_offset)
{
  /* Select the offseted indirect register's. */
  wrmem32((addr_t) APIC_IO_REG_INDEX, reg_offset);

  /* Return the content of the selected indirect register. */
  return rdmem32((addr_t) APIC_IO_REG_DATA);
}


/**
 * _apic_io_wrreg():
 *
 */
static void		_apic_io_wrreg(uint32_t	reg_offset,
				       uint32_t	reg_value)
{
   /* Write some 32bit data to an offseted indirect register. */
  wrmem32((addr_t)(APIC_IO_REG_DATA), reg_value);

  /* Select that offseted indirect register to eventually make the write. */
  wrmem32((addr_t) APIC_IO_REG_INDEX, reg_offset);
}


/**
 * _apic_io_get_id():
 *
 */
static uint8_t		_apic_io_get_id(void)
{
  /* Return the IO-APIC id value. */
  return (uint8_t)((_apic_io_rdreg(APIC_IO_REG_OFFSET_ID) >> 24) & 0xf);
}


/**
 * _apic_io_get_version():
 *
 */
static uint32_t		_apic_io_get_version(void)
{
  /* Return the IO-APIC version number. */
  return (uint32_t) _apic_io_rdreg(APIC_IO_REG_OFFSET_VERSION);
}


/**
 * apic_io_ack():
 *
 * Acknowledgement by writing the EOI register of the IO-APIC is required to
 * de-assert level-triggered interrupts (not required for edge-triggered ones).
 */


/**
 * _apic_io_add_redirection_entry():
 *
 */
static void		_apic_io_add_redentry(uint8_t	red_n,
					      uint8_t	int_n)
{
  uint64_t		entry = 0ull;

  /**
   * Interrupt configuration:
   *
   * - Delivery mode: fixed
   * - Destination mode: physical
   * - Pin polarity: active high
   * - Trigger mode: edge-triggered
   * - Mask: unmasked
   */
  entry |= int_n /* | APIC_IO_REG_FLAG_REDTBL_MASKED */;

  /* Write the high-order bits of the redirection table register. */
  _apic_io_wrreg(red_n, (uint32_t)(entry >> 32));

  /* Write the low-order bits of the redirection table register. */
  _apic_io_wrreg(red_n + 1, (uint32_t) entry);
}
