/* mandatory includes */
#include "types.h"
#include "kernel.h"
#include "tty.h"

/* additional includes */
#include "stdio.h"
#include "stdlib.h"
#include "console.h"

#ifdef __NO_COMPILE__

static s_tty	tty[TTY_N];

int32_t	tty_init(void)
{
uint32_t	i = 0;
  char		filename[STDIO_BF_SIZE_SMALL];

  for (i = 0; i < TTY_N; i++)
    {
      memzero(filename, STDIO_BF_SIZE_SMALL);
      sprintf(filename, "tty%d", i);
      tty[i].desc = fopen(filename, "r+c");
      tty[i].active = false;
      tty[i].lastline = 0;
    }

  return ERR_NONE;
}

int32_t	tty_refresh(void)
{
uint32_t	i = 0;

  /* find an active tty */
  while ((i < TTY_N) && (tty[i].active == false))
    i++;

  /* if no tty are currently active, return with an error */
  if (i >= TTY_N)
    return ERR_UNKNOWN;

  /* refresh the console with the current tty output */
  tty[i].lastline = console_refresh(tty[i].desc, tty[i].lastline);

  return ERR_NONE;
}

int32_t	tty_activate(uint8_t	n)
{
uint32_t	i = 0;

  /* argument checking */  
  if (n > TTY_N)
    return ERR_UNKNOWN;

  /* find an active tty, and deactivate it */
  while ((i < TTY_N) && (tty[i].active == false))
    i++;
  if ((i < TTY_N) && (tty[i].active == true))
    tty[i].active = false;

  /* finally, activate the requested tty output */
  tty[n].active = true;

  return ERR_NONE;
}

#endif
