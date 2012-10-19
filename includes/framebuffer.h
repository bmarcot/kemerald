#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#define FB_BASE_ADDR	0xb8000

#define FB_COLUMN_N	80
#define FB_LINE_N	25

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


int32_t	fb_cls(void);
int32_t	fb_scrolldown(void);
int32_t	fb_println(char *, char);

int32_t	_fb_print_checkerboard(void);
int32_t	_fb_print_chartable(void);
int32_t	_fb_print_colortable(void);
int32_t	fb_printmem(void *,uint32_t);

#endif /* !__FRAMEBUFFER_H__ */
