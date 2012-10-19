/* mandatory includes */
#include "types.h"
#include "kernel.h"
#include "stdio.h"

/* additional includes */
#include "stdlib.h"
//#include "stdarg.h"
#include "string.h"
#include "console.h" // a dégager dans le driver ! > meme plus: a degager tout court car printf passe par fprintf(). La mise a jour du buffer video a lieu dans une interruption

char		stdio_filebuf[STDIO_FILEDES_N][STDIO_BF_SIZE_LARGE]
__attribute__ ((section (".filedes")));

FILE		stdio_filedes[STDIO_FILEDES_N]
__attribute__ ((section (".filedes")));

int32_t	stdio_init(void)
{
uint32_t	i = 0;

  /* initialize the allocatable filedes structure */
  for (i = 0; i < STDIO_FILEDES_N; i++)
    {
      setvbuf(&(stdio_filedes[i]),
	      stdio_filebuf[i],
	      _IOFBF,
	      STDIO_BF_SIZE_LARGE);
      stdio_filedes[i].buffer.offset = 0;
      memzero(stdio_filedes[i].buffer.data, STDIO_BF_SIZE_LARGE);

      stdio_filedes[i].fileoffset = 0;
      stdio_filedes[i].filesize = 0;
      // stdio_filedes[i].filename
      stdio_filedes[i].fileid = -1;
      stdio_filedes[i].filemode = 0;
    }

  // XXX: A faire dans le console_init():
  //	réserver n fichiers correspondant aux consoles !!
  /* reserve the tty0-tty3 console */
#ifdef KLHDQSJLJD
  for (i = 0; i < STDIO_TTY_N; i++)
    {
      /*stdio_filedes[i].offset =
	sprintf(stdio_filedes[i].buffer, "tty%d\n", i);*/
      stdio_filedes[i].buffer.mode |= _IOCBF;

      stdio_filedes[i].filesize = STDIO_BF_SIZE_LARGE;
      // stdio_filedes[i].filename
      stdio_filedes[i].fileid = i;
    }
#endif

  return ERR_NONE;
}

/* int32_t	printf(const char	*format, */
/* 		       ...) */
/* { */
/*   char		output[STDIO_LARGE_BUFFER_S]; */
/*   va_list	arg; */

/*   memzero(output, STDIO_LARGE_BUFFER_S); */
/*   va_start(arg, format); */
/*   vsprintf(output, format, arg); // a remplacer par vfprintf() */
/*   //console_printf(output, BG_BLACK | FG_WHITE); */
/*   va_end(arg); */

/*   return 0; */
/* } */

/**
 * Warning:
 * No verification is made on the string s parameter. This parameter should have
 * been zeroed, and sufficient memory must have been allocated for the formated
 * string.
 */
int32_t		sprintf(char		*s,
			const char	*format,
			...)
{
  int32_t	status = ERR_NONE;
  va_list	arg;

  if (s)
    {
      if (strlen(format))
	{
	  va_start(arg, format);
	  status = vsprintf(s, format, arg);
	  va_end(arg);
	}
    }
  else
    status = ERR_UNKNOWN;

  return status;
}

int32_t	fprintf(FILE		*stream,
			const char	*format,
			...)
{
 int32_t	status = ERR_NONE;
  va_list	arg;

  if (stream && (stream->fileid >= 0))
    {
      if (strlen(format))
	{
	  va_start(arg, format);
	  status = vfprintf(stream, format, arg);
	  stream->buffer.data[stream->buffer.offset] = '\0';
	  va_end(arg);
	}
    }
  else
    status = ERR_UNKNOWN;

  return status;
}

int32_t	vfprintf(FILE		*stream,
			 const char	*format,
			 va_list	arg)
{
 int32_t	v = 0;
uint32_t	i = 0;
uint32_t	j = 0;
uint32_t	k = 0;
  char		buf[STDIO_BF_SIZE_MEDIUM];
  char		reg[9];

  reg[8] = '\0';

  for (i = 0; i < strlen(format); i++)
    {
      if (format[i] != '%')
	{
	  stream->buffer.data[stream->buffer.offset++] = format[i];
	  stream->fileoffset++;
	}
      else
	{
	  memzero(buf, STDIO_BF_SIZE_MEDIUM);
	  switch (format[++i])
	    {
	      /* signed decimal */
	    case 'd':
	      v = va_arg(arg, int32_t);
	      j += itoa(buf, v, STRING_FORMAT_DEC);
	      STDIO_FPRINTF_STRCAT(buf);
	      break;

	      /* unsigned decimal */
	    case 'u':
	      v = va_arg(arg,uint32_t);
	      j += itoa(buf, (uint32_t)v, STRING_FORMAT_DEC);
	      STDIO_FPRINTF_STRCAT(buf);
	      break;

	      /* hexadecimal */
	    case 'x':
	      v = va_arg(arg,uint32_t);
	      STDIO_FPRINTF_STRCAT("0x");
	      j += (itoa(buf, (uint32_t)v, STRING_FORMAT_HEX) + 2);
	      STDIO_FPRINTF_STRCAT(buf);
	      //console_printf(buf, BG_BLACK | FG_YELLOW | FG_INTENSITY);
	      break;

	      /* register */
	    case 'r':
	      v = va_arg(arg,uint32_t);
	      STDIO_FPRINTF_STRCAT("0x");
	      itoa(buf, (uint32_t)v, STRING_FORMAT_HEX);
	      memset(reg, '0', 8);
	      for (k = 0; k < strlen(buf); k++)
		reg[k + 8 - strlen(buf)] = buf[k];
	      j+= 10;
	      STDIO_FPRINTF_STRCAT(reg);
	      break;

	      /* percent sign */
	    case '%':
	      stream->buffer.data[stream->buffer.offset++] = '%';
	      stream->fileoffset++;
	      break;

	    default:
	      return ERR_UNKNOWN;
	    }
	}

      if ((stream->buffer.mode & _IOCBF)
	  && (stream->buffer.offset >= stream->buffer.size))
	stream->fileoffset = stream->buffer.offset = 0;
    }

  return j;
}

int32_t	fstrcat(FILE		*stream,
			const char	*s)
{
uint32_t	i = 0;

  /* argument checking */
  if ((stream == NULL) || (s == NULL))
    return ERR_NULLPTR;

  while (i < strlen(s))
    {
      stream->buffer.data[stream->buffer.offset++] = s[i++];
      stream->fileoffset++;

      if ((stream->buffer.mode & _IOCBF)
	  && (stream->buffer.offset >= stream->buffer.size))
	stream->fileoffset = stream->buffer.offset = 0;
    }

  return ERR_NONE;
}

int32_t	vsprintf(char		*s,
			 const char	*format,
			 va_list	arg)
{
 int32_t	v;
uint32_t	i;
uint32_t	j = 0;
uint32_t	k;
  char		buffer[STDIO_BF_SIZE_MEDIUM];

  for (i = 0; i < strlen(format); i++)
    {
      if (format[i] != '%')
	s[j++] = format[i];
      else
	{
	  memzero(buffer, STDIO_BF_SIZE_MEDIUM);
	  switch (format[++i])
	    {
	      /* signed decimal */
	    case 'd':
	      v = va_arg(arg, int32_t);
	      j += itoa(buffer, v, STRING_FORMAT_DEC);
	      strcat(s, buffer);
	      break;

	      /* unsigned decimal */
	    case 'u':
	      v = va_arg(arg,uint32_t);
	      j += itoa(buffer, (uint32_t)v, STRING_FORMAT_DEC);
	      strcat(s, buffer);
	      break;

	      /* hexadecimal */
	    case 'x':
	      v = va_arg(arg,uint32_t);
	      strcat(s, "0x");
	      j += (itoa(buffer, (uint32_t)v, STRING_FORMAT_HEX) + 2);
	      strcat(s, buffer);
	      break;

	      /* register */
	    case 'r':
	      v = va_arg(arg,uint32_t);
	      strcat(s, "0x");
	      itoa(buffer, (uint32_t)v, STRING_FORMAT_HEX);
	      for (k = 0; k < 8 - strlen(buffer); k++)
		s[j + k + 2] = '0';
	      strcat(s, buffer);
	      j+= 10;
	      break;

	      /* percent sign */
	    case '%':
	      s[j++] = '%';
	      break;

	    default:
	      return -1;
	    }
	}
    }

  return j;
}

int32_t	fseek(FILE	*stream,
 int64_t	offset,
 int32_t	whence)
{
  /* argument checking */
  if (stream == NULL)
    return ERR_NULLPTR;

  switch (whence)
    {
    case SEEK_SET:
      /* if (offset >= stream->filesize) */
      /* 	return ERR_UNKNOWN; */
      stream->fileoffset = offset;
      /**
       * si (offset > buffer.offset)
       * alors recharger le buffer (fonction read)
       */
      stream->buffer.offset = offset;
      break;
    case SEEK_CUR:
#ifdef __NOT_COMPILED__
      if ((stream->offset + offset) >= stream->filesize)
	return ERR_UNKNOWN;
      /* XXX: gestion mono buffer... dans le cadre de vrai fichier il faudra
       * recharger le buffer.
       */
      if (stream->buffer.mode & _IOCBF)
	stream->offset = (stream->offset + offset) % stream->buffer->size; 
      else
	{
	  if ((stream->offset + offset) >= stream->buffer->size)
	    return ERR_NONE;
	  else
	    stream->offset += offset;
	}
#endif /* __NOT_COMPILED__ */
      break;
    case SEEK_END:
      // XXX: to be implemented
      break;
    default:
      return ERR_UNKNOWN;
    }

  return ERR_NONE;
}

char		*fgets(char	*s,
 int32_t	n,
		       FILE	*stream)
{
uint32_t	i = 0;
  bool		eol = false;

  if (stream && (stream->buffer.data[stream->buffer.offset] != EOF))
    {
      while (!eol
	     && (i < (n - 1))
	     && (stream->buffer.data[stream->buffer.offset] != EOF))
	{
	  if (stream->buffer.data[stream->buffer.offset] == '\n')
	    eol = true;
	  
	  s[i++] = stream->buffer.data[stream->buffer.offset++];
	  stream->fileoffset++;
	 
	  // circular buffer management 
	  if ((stream->buffer.mode & _IOCBF)
	      && (stream->buffer.offset >= stream->buffer.size))
	    stream->fileoffset = stream->buffer.offset = 0;
	}

      // est ce que fgets doit rajouter le char \0 ? vérifier dans la norme
      /*s[i] = '\0';*/
    }
  else
    return NULL;

  return s;
}

int64_t	ftell(FILE	*stream)
{
  /* arguments checking */
  if (stream == NULL)
    return ERR_NULLPTR;

  return stream->fileoffset;
  //return stream->buffer.offset;
}

int32_t	setvbuf(FILE	*stream,
			char	*buf,
	int32_t	mode,
			unsigned long	size)
{
  /* arguments checking */
  if (stream == NULL)
    return ERR_NULLPTR;

  if (buf == NULL)
    {
      stream->buffer.data = NULL;
      stream->buffer.size = 0;
      stream->buffer.mode = _IONBF;
    }
  else
    {
      stream->buffer.data = buf;
      stream->buffer.size = size;
      stream->buffer.mode = mode;
    }

  return ERR_NONE;
}

FILE		*fopen(const char	*filename,
		       const char	*mode)
{
uint32_t	i = 0;
uint32_t	j = 0;

  /* arguments checking */
  if ((filename == NULL) || (mode == NULL))
    return NULL;

  /* find a free file descriptor */
  while ((i < STDIO_FILEDES_N) && (stdio_filedes[i].fileid != -1))
    i++;

  /* if no file descriptor is available, return with an error */
  if (i == STDIO_FILEDES_N)
    return NULL;

  /* reserve this file descriptor by setting the filename and the fileid */
  strcpy(stdio_filedes[i].filename, filename);
  stdio_filedes[i].fileid = i;

  /* reinit the descriptor and buffer parameters */
  stdio_filedes[i].fileoffset = 0;
  stdio_filedes[i].filesize = STDIO_BF_SIZE_LARGE;
  setvbuf(&(stdio_filedes[i]),
	  stdio_filebuf[i],
	  _IOFBF,
	  STDIO_BF_SIZE_LARGE);
  stdio_filedes[i].buffer.offset = 0;
  
  // XXX: dans les options de fopen() plus bas...
  //stdio_filedes[i].buffer.mode = _IOCBF;

  memzero(stdio_filedes[i].buffer.data, STDIO_BF_SIZE_LARGE);

  //#ifdef NOT_COMPILED
  /* set the file mode according to the mode argument */
  j = 0;
  while (j < strlen(mode))
    {
      switch (mode[j])
	{
	  /* read mode */
	case 'r':
	  stdio_filedes[i].filemode |= STDIO_FILE_MODE_READ;
	  break;

	  /* write mode */
	case 'w':
	  stdio_filedes[i].filemode |= STDIO_FILE_MODE_WRITE;
	  break;

	  /* XXX: + mode */
	case '+':
	  break;

	  /* XXX: a mode */
	case 'a':
	  break;

	  /* circular file */
	case 'c':
	  stdio_filedes[i].buffer.mode = _IOCBF;
	  break;

	  /* unknown option */
	default:
	  return NULL;
	}
      j++;
    }
  //#endif

  return &(stdio_filedes[i]);
}

int32_t	printf(const char	*format,
		       ...)
{
 int32_t	status = ERR_NONE;
  va_list	arg;
  char		buffer[STDIO_BF_SIZE_MEDIUM];

  memzero(buffer, STDIO_BF_SIZE_MEDIUM);

  if (format)
    {
      if (strlen(format))
	{
	  va_start(arg, format);
	  status = vsprintf(buffer, format, arg);
	  va_end(arg);
	}
    }
  else
    return ERR_UNKNOWN;

  status = console_printf(buffer, FG_WHITE | BG_BLACK/* | FG_INTENSITY*/);

  return status;
}
