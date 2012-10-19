#ifndef __STRING_H__
#define __STRING_H__

#define HEX_BASE		"0123456789abcdef"
#define STRING_FORMAT_BIN	2
#define STRING_FORMAT_OCT	8
#define STRING_FORMAT_DEC	10
#define STRING_FORMAT_HEX	16

//extern char	g_base[];

int32_t	strlen(const char *);
int32_t	strcat(char *, const char *);
int32_t	strcpy(char *, const char *);
int32_t	itoa(char *, uint32_t, uint8_t);

#endif /* !__STRING_H__ */
