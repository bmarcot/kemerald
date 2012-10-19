/**
 * Mandatory includes
 *
 */
#include "types.h"
#include "kernel.h"
#include "utils.h"


/**
 * rdcr0():
 *
 */
__inline__ uint32_t	rdcr0(void)
{
  uint32_t		cr0;

  RDCR0(&cr0);

  return cr0;
}


/**
 * rdcr1():
 *
 */
__inline__ uint32_t	rdcr1(void)
{
  uint32_t		cr1;

  RDCR2(&cr1);

  return cr1;
}


/**
 * rdcr2():
 *
 */
__inline__ uint32_t	rdcr2(void)
{
  uint32_t		cr2;

  RDCR2(&cr2);

  return cr2;
}


/**
 * rdcr3():
 *
 */
__inline__ uint32_t	rdcr3(void)
{
  uint32_t		cr3;

  RDCR2(&cr3);

  return cr3;
}


/**
 * rdcr4():
 *
 */
__inline__ uint32_t	rdcr4(void)
{
  uint32_t		cr4;

  RDCR4(&cr4);

  return cr4;
}


/**
 * rdmem32():
 *
 */
__inline__ uint32_t	rdmem32(volatile addr_t	addr)
{
  return *(uint32_t *) addr;
}


/**
 * wrmem32():
 *
 */
__inline__ void		wrmem32(volatile addr_t	addr,
				uint32_t	data)
{
  *(uint32_t *) addr = data;
}


/**
 * rdesp():
 *
 */
__inline__ uint32_t	rdesp(void)
{
  uint32_t		esp;

  RDESP(&esp);

  return esp;
}


/**
 * rdebp():
 *
 */
__inline__ uint32_t	rdebp(void)
{
  uint32_t		ebp;

  RDEBP(&ebp);

  return ebp;
}


/**
 * hlt():
 *
 */
__inline__ void		hlt(void)
{
  HLT();
}


/**
 * cli():
 *
 */
__inline__ void		cli(void)
{
  CLI();
}


/**
 * sti():
 *
 */
__inline__ void		sti(void)
{
  STI();
}


/**
 * lgdt():
 *
 */
__inline__ void		lgdt(addr_t	addr)
{
  LGDT(addr);
}


/**
 * lidt():
 *
 */
__inline__ void		lidt(addr_t	addr)
{
  LIDT(addr);
}


/**
 * ltr():
 *
 */
__inline__ void		ltr(uint16_t	selector)
{
  LTR(selector);
}


/**
 * bsr():
 *
 * Return the most-significant bit set in a register.
 */
__inline__ int8_t	bsr(uint32_t	r)
{
  uint32_t		index;
  uint32_t		zf;

  /* Call the Bit Search Reverse instruction. */
  BSR(r, &index, &zf);

  /* If a bit set has been found, return its index. */
  if (!zf)
    {
      return index;
    }

  /* The register contains only 0, then return -1. */
  return ERR_UNKNOWN;
}


/**
 * bsf():
 *
 * Return the least-significant bit set in a register.
 */
__inline__ int8_t	bsf(uint32_t	r)
{
  uint32_t		index;
  uint32_t		zf;

  /* Call the Bit Search Forward instruction. */
  BSF(r, &index, &zf);

  /* If a bit set has been found, return its index. */
  if (!zf)
    {
      return index;
    }

  /* The register contains only 0, then return -1. */
  return ERR_UNKNOWN;
}
