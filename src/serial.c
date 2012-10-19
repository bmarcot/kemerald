/* mandatory includes */
#include "types.h"
#include "kernel.h"
#include "serial.h"

static bool	_serial[SERIAL_N];

int32_t		serial_init(void)
{
  uint32_t	i;

  for (i = 0; i < SERIAL_N; i++)
    {
      _serial[i] = false;
    }

  return ERR_NONE;
}

int32_t		serial_generate(void)
{
  uint32_t	i;

  for (i = 0; i < SERIAL_N; i++)
    {
      if (_serial[i] == false)
	{
	  _serial[i] = true;
	  return i;
	}
    }

  return ERR_UNKNOWN;
}

int32_t		serial_free(uint32_t	i)
{
  if (i >= SERIAL_N)
    return ERR_UNKNOWN;

  _serial[i] = false;

  return ERR_NONE;
}
