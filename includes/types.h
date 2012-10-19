#ifndef __TYPES_H__
#define __TYPES_H__

typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long long int	uint64_t;

typedef signed char		int8_t;
typedef signed short		int16_t;
typedef signed int		int32_t;
typedef signed long long int	int64_t;

typedef unsigned char		uchar;

typedef enum _bool
  {
    false = 0,
    true
  } bool;

typedef void*			addr_t;
typedef void*			paddr_t;	
typedef void*			vaddr_t;

#endif /* !__TYPES_H__ */
