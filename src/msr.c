/**
 * Mandatory includes
 *
 */
#include "types.h"
#include "msr.h"


/**
 * rdmsr():
 *
 */
__inline__ uint64_t	rdmsr(uint32_t		id)
{
  uint64_t	value;

  RDMSR3(id, &value);

  return value;
}


/**
 * wrmsr():
 *
 */
__inline__ void		wrmsr(uint32_t		id,
			      uint64_t		value)
{
  WRMSR3(id, value);
}
