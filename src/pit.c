/* mandatory includes */
#include "types.h"
#include "kernel.h"
#include "pit.h"

/* additional includes */
#include "port.h"

int32_t		pit_init(void)
{
  /**
   * configure the Programmable Interrupt Timer with these parameters :
   *	- Channel 0
   *	- send LSB, then send MSB
   *	- Square Wave mode
   *	- no BCD
   */
  outb(PORT_PIT_COMMAND, 0x36);

  /* Set the frequency to 1Hz. */
  pit_set_frequency(1193180 / 100);

  return ERR_NONE;
}

int32_t		pit_set_frequency(uint32_t	freq)
{
  /* Firstly, send the divisor's value least significant byte. */
  outb(PORT_PIT_DATA, (uint8_t)(freq & 0xff));

  /* Secondly, send the divisor's value most significant byte. */
  outb(PORT_PIT_DATA, (uint8_t)(freq >> 8));

  return ERR_NONE;
}
