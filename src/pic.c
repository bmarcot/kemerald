#include "types.h"
#include "kernel.h"
#include "port.h"

/*
 * hardware component : 8259A
 */

int32_t		pic_enable(bool		b)
{
  uint8_t	bitmap = 0;

  if (b == false)
    bitmap = ~0;

  outb(PORT_PIC_MASTER_DATA, bitmap);
  outb(PORT_PIC_SLAVE_DATA, bitmap);

  return ERR_NONE;
}
