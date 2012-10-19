#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "stdio.h"

#define FG_BLACK	0x0
#define FG_BLUE		0x1
#define FG_GREEN	0x2
#define FG_CYAN		0x3
#define FG_RED		0x4
#define FG_MAGENTA	0x5
#define FG_YELLOW	0x6
#define FG_WHITE	0x7
#define FG_INTENSITY	0x8
#define BG_BLACK	(FG_BLACK << 4)
#define BG_BLUE		(FG_BLUE << 4)
#define BG_GREEN	(FG_GREEN << 4)
#define BG_CYAN		(FG_CYAN << 4)
#define BG_RED		(FG_RED << 4)
#define BG_MAGENTA	(FG_MAGENTA << 4)
#define BG_YELLOW	(FG_YELLOW << 4)
#define BG_WHITE	(FG_WHITE << 4)
#define BG_PASTEL	(FG_INTENSITY << 4)

#define CONSOLE_COLUMN_N	80
#define CONSOLE_LINE_N		25
#define CONSOLE_VIDEO_BUFFER_S	2 * CONSOLE_COLUMN_N * CONSOLE_LINE_N
#define CONSOLE_TABULATION_N	8
#define CONSOLE_GET_INDEX	((g_console_line * CONSOLE_COLUMN_N	\
				 + g_console_column) * 2)

int32_t	console_init(void);
int32_t	console_clear(void);
int32_t	console_scrolldown(void);
int32_t	console_printf(char *, char);
//void	console_refresh(void);
int32_t	console_refresh(FILE *,uint32_t);

/* define an alias for printf() function */
//#define printf		console_printf

/* define an alias for clear() function */
#define clear		console_clear


void write_char(uint8_t);

#endif /* !__CONSOLE_H__ */
