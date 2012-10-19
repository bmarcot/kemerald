/**
 * Mandatory includes
 *
 */
#include "types.h"
#include "port.h"


/**
 * outb():
 *
 */
__inline__ void		outb(uint16_t	port,
			     uint8_t	val)
{
  OUTB(val, port);
}


/**
 * outw():
 *
 */
__inline__ void		outw(uint16_t	port,
			     uint16_t	val)
{
  OUTW(val, port);
}


/**
 * outl():
 *
 */
__inline__ void		outl(uint16_t	port,
			     uint32_t	val)
{
  OUTL(val, port);
}


/**
 * inb():
 *
 */
__inline__ uint8_t	inb(uint16_t	port)
{
  uint8_t	val;

  INB(&val, port);

  return val;
}


/**
 * inw():
 *
 */
__inline__ uint16_t	inw(uint16_t	port)
{
  uint16_t	val;

  INW(&val, port);

  return val;
}


/**
 * inl():
 *
 */
__inline__ uint32_t	inl(uint16_t	port)
{
  uint32_t	val;

  INL(&val, port);

  return val;
}


