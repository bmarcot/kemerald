#include "types.h"
#include "kernel.h"
#include "console.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

static uint32_t	g_console_column;
static uint32_t	g_console_line;
static uint8_t	g_console_video[CONSOLE_VIDEO_BUFFER_S]
__attribute__ ((section (".console_video")));

static uint64_t	console_lastline;

int32_t	console_init(void)
{
uint32_t	i = 0;

  /* reset column and line to 0 */
  g_console_column = g_console_line = 0;

  /* init static and global variable */
  console_lastline = 0;

  console_clear();
  for (i = 1; i < CONSOLE_VIDEO_BUFFER_S; i+=2) 
    g_console_video[i] = FG_WHITE | BG_BLACK | FG_INTENSITY;

  return ERR_NONE;
}

int32_t	console_clear(void)
{
  /* clear the video buffer memory */
  memzero((void *)g_console_video, CONSOLE_VIDEO_BUFFER_S);

  /* reset column and line to 0 */
  g_console_column = g_console_line = 0;

  return ERR_NONE;
}

int32_t	console_scrolldown(void)
{
  memcpy((void *)g_console_video,
	 (void *)(&g_console_video[CONSOLE_COLUMN_N * 2]),
	 CONSOLE_COLUMN_N * (CONSOLE_LINE_N - 2) * 2);

  memzero((void *)(&g_console_video[CONSOLE_COLUMN_N
				    * (CONSOLE_LINE_N - 2) * 2]),
	  CONSOLE_COLUMN_N * 2);

  return 0;
}

int32_t	console_printf(char	*str,
		       char	attribs)
{
uint32_t	i = 0;
uint32_t	length;

  if (str == NULL)
    return ERR_UNKNOWN;

  if (g_console_line >= CONSOLE_LINE_N - 1)
    {
      console_scrolldown();
      g_console_line--;
    }

  length = strlen(str);

  if (length)
    {
      while ((i < length) && (i < CONSOLE_COLUMN_N))
	{
	  switch (str[i])
	    {
	      /* escaped : new line*/
	    case '\n':
	      g_console_column = 0;
	      g_console_line++;
	      break;

	      /* escaped : tabulation */
	    case '\t':
	      g_console_column = (g_console_column / CONSOLE_TABULATION_N)
		* CONSOLE_TABULATION_N + CONSOLE_TABULATION_N;
	      break;

	      /* default case : normal character */
	    default:
	      g_console_video[CONSOLE_GET_INDEX] = str[i];
	      g_console_video[CONSOLE_GET_INDEX + 1] = attribs;

	      g_console_column++;
	      if (g_console_column >= CONSOLE_COLUMN_N)
		{
		  g_console_column = 0;
		  g_console_line++;
		}
	    }
	  i++;
	}
    }

  return ERR_NONE;
}

int32_t	console_refresh(FILE	*stream,
			uint32_t	lastline)
{
  char		line[CONSOLE_COLUMN_N];

  /* argument checking */  
  if (stream == NULL)
    return ERR_NULLPTR;

  if (fseek(stream, lastline, SEEK_SET) != ERR_NONE)
    return ERR_UNKNOWN;
  memzero(line, CONSOLE_COLUMN_N);
  while (NULL != fgets(line, CONSOLE_COLUMN_N, stream))
    {
      console_printf(line, BG_BLACK | FG_WHITE);
      memzero(line, CONSOLE_COLUMN_N);
    }

  return ftell(stream);
}



void write_char(uint8_t c)
{
  g_console_video[0] = c;
  g_console_video[1] = BG_YELLOW | FG_BLACK | BG_PASTEL;
}
