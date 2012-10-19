#ifndef __STDIO_H__
#define __STDIO_H__

#ifndef __STDARG_H__
#include "stdarg.h"
#endif

#define STDIO_LARGE_BUFFER_S	64 // a degager,remplacé par les define plus bas
#define STDIO_SMALL_BUFFER_S	16 //  "    "
#define STDIO_FILE_BUFFER_S	512 // "    "

#define STDIO_FILEDES_N		1
#define STDIO_TTY_N		4 // a degager dans le driver !

#define stdout			(&stdio_filedes[0])
#define stdint			(&stdio_filedes[1])

//#define O_NONE			0 // ca degage
//#define O_CIRCULAR		(1 << 7)

#define STDIO_BF_TYPE_SEQUENTIAL	0
#define STDIO_BF_TYPE_CIRCULAR		1

#define STDIO_BF_SIZE_LARGE		512
#define STDIO_BF_SIZE_MEDIUM		128
#define STDIO_BF_SIZE_SMALL		16

#define STDIO_FILE_MODE_READ		(1 << 0)
#define STDIO_FILE_MODE_WRITE		(1 << 1)

/* POSIX compliancy */

#define SEEK_SET		0
#define SEEK_CUR		1
#define SEEK_END		2

#define _IOFBF			(1 << 0)	/* Full buffering */
#define _IOLBF			(1 << 1)	/* Line buffering */
#define _IONBF			(1 << 2)	/* No buffering */
#define _IOCBF			(1 << 3)	/* Circular buffering */

#define EOF			0

/* !POSIX compliancy */

/*#define printf(FORMAT, ...)			\
  fprintf(stdout, FORMAT, __VA_ARGS__);*/

#define STDIO_FPRINTF_STRCAT(BUF)			\
  if (stream->buffer.mode & _IOCBF)			\
    fstrcat(stream, BUF);				\
  else							\
    strcat(stream->buffer.data, BUF);
  
typedef struct	_s_buffer
{
  uchar		*data;
uint32_t	offset;
uint32_t	size;
uint32_t	mode;
} s_buffer;

typedef struct	_s_file
{
  s_buffer	buffer;
uint32_t	fileoffset;
uint64_t	filesize;
  char		filename[16];
 int32_t	fileid;
uint16_t	filemode;
} FILE;

int32_t	stdio_init(void);
//int32	printf(const char *, ...);
int32_t	sprintf(char *, const char *, ...);
int32_t	fprintf(FILE *, const char *, ...);
int32_t	vsprintf(char *, const char *, va_list);
int32_t	vfprintf(FILE *, const char *, va_list);
int32_t	fseek(FILE *, int64_t,int32_t);
char	*fgets(char *, int32_t,FILE *);
int64_t	ftell(FILE *);
FILE	*fopen(const char *, const char *);

/* XXX: to be implemented */
int setvbuf(FILE *stream, char *buf, int32_t mode, /*size_t*/unsigned long size);

int32_t	fstrcat(FILE *, const char *);

int32_t	printf(const char *, ...);

extern FILE stdio_filedes[];

#endif /* !__STDIO_H__ */
