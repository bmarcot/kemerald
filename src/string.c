/* mandatory includes */
#include "types.h"
#include "kernel.h"
#include "string.h"

/* additional includes */
#include "stdlib.h"

//char		_g_base[] = HEX_BASE;

int32_t	strlen(const char	*str)
{
uint32_t	i = 0;

  while (str[i])
    i++;
    
  return i;
}


int32_t	strcat(char		*dest,
		       const char	*src)
{
uint32_t	i = 0;
uint32_t	j = 0;

  while (dest[i])
    i++;

  while (src[j])
    dest[i++] = src[j++];

  return 0;
}

int32_t		strcpy(char		*dest,
		       const char	*src)
{
uint32_t	i = 0;

  /* arguments checking */
  if ((dest == NULL) || (src == NULL))
    return ERR_NULLPTR;

  while (src[i])
    {
      dest[i] = src[i];
      i++;
    }

  return ERR_NONE;
}


int32_t		itoa(char	*buffer,
		     uint32_t	n,
		     uint8_t	base_length)
{
  char		c;
  char		*base = HEX_BASE;
  uint32_t	i = 0;
  uint32_t	position = 0;

  while (n / base_length)
    {
      buffer[position++] = base[n % base_length];
      n /= base_length;
    }
  buffer[position] = base[n];

  while ((i <= (position / 2)) && (position > 0))
    {
      c = buffer[i];
      buffer[i] = buffer[position - i];
      buffer[position - i] = c;
      i++;
    }

  return ++position;
}
