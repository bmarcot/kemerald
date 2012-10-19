#ifndef __TTY_H__
#define __TTY_H__

#include "stdio.h"

#define TTY_N		4

typedef struct _s_tty {
  FILE		*desc;
  bool		active;
uint32_t	lastline;
} s_tty;

int32_t	tty_init(void);
int32_t	tty_refresh(void);
int32_t	tty_activate(uint8_t);

#endif /* !__TTY_H__ */
