#ifndef __UTILS_H__
#define __UTILS_H__

/**
 * Defines
 *
 */

#define MASK_8BITS		0xff
#define MASK_16BITS		0xffff
#define MASK_32BITS		0xffffffff


/**
 * Macros
 *
 */

#define HLT()							\
  __asm__ __volatile__ ("hlt");

#define CLI()							\
  __asm__ __volatile__ ("cli");

#define STI()							\
  __asm__ __volatile__ ("sti");

#define RDMEM32(_data_, _addr_)					\
  __asm__ __volatile__ ("	movl	(%1), %0"		\
			: "=r" (*_data_)			\
			: "r" (_addr_)				\
			);

#define WRMEM32(_data_, _addr_)					\
  __asm__ __volatile__ ("	movl	%0, (%1)"		\
			:					\
			: "r" (_data_), "r" (_addr_)		\
			);

#define RDESP(_value_)						\
  __asm__ __volatile__ ("	movl	%%esp, %0"		\
			: "=r" (*_value_)			\
			);

#define RDEBP(_value_)						\
  __asm__ __volatile__ ("	movl	%%ebp, %0"		\
			: "=r" (*_value_)			\
			);

#define RDCR0(_value_)						\
  __asm__ __volatile__ ("	movl	%%cr0, %0"		\
			: "=r" (*_value_)			\
			);

#define RDCR1(_value_)						\
  __asm__ __volatile__ ("	movl	%%cr1, %0"		\
			: "=r" (*_value_)			\
			);

#define RDCR2(_value_)						\
  __asm__ __volatile__ ("	movl	%%cr2, %0"		\
			: "=r" (*_value_)			\
			);

#define RDCR3(_value_)						\
  __asm__ __volatile__ ("	movl	%%cr3, %0"		\
			: "=r" (*_value_)			\
			);

#define RDCR4(_value_)						\
  __asm__ __volatile__ ("	movl	%%cr4, %0"		\
			: "=r" (*_value_)			\
			);

#define LGDT(_addr_)						\
  __asm__ __volatile__ ("	lgdtl	(%0)"			\
			:					\
			: "p" (_addr_)				\
			);

#define LIDT(_addr_)						\
  __asm__ __volatile__ ("	lidtl	(%0)"			\
			:					\
			: "p" (_addr_)				\
			);

#define LTR(_selector_)						\
  __asm__ __volatile__ ("	ltrw	%0"			\
			:					\
			: "r" (_selector_)			\
			);

#define BSR(_value_, _index_, _zf_)				\
  __asm__ __volatile__ ("	movl	$0, %1		\n\t"	\
			"	bsrl	%2, %0		\n\t"	\
			"	jnz	0f		\n\t"	\
			"	movl	$1, %1		\n\t"	\
			"0:"					\
			: "=r" (*_index_), "=r" (*_zf_)		\
			: "r" (_value_)				\
			);

#define BSF(_value_, _index_, _zf_)				\
  __asm__ __volatile__ ("	movl	$0, %1		\n\t"	\
			"	bsfl	%2, %0		\n\t"	\
			"	jnz	0f		\n\t"	\
			"	movl	$1, %1		\n\t"	\
			"0:"					\
			: "=r" (*_index_), "=r" (*_zf_)		\
			: "r" (_value_)				\
			);


/**
 * Forward declarations
 *
 */
__inline__ uint32_t	rdcr0(void);
__inline__ uint32_t	rdcr1(void);
__inline__ uint32_t	rdcr2(void);
__inline__ uint32_t	rdcr3(void);
__inline__ uint32_t	rdcr4(void);
__inline__ uint32_t	rdesp(void);
__inline__ uint32_t	rdebp(void);
__inline__ uint32_t	rdmem32(addr_t);
__inline__ void		wrmem32(addr_t, uint32_t);
__inline__ void		hlt(void);
__inline__ void		cli(void);
__inline__ void		sti(void);
__inline__ void		lgdt(addr_t);
__inline__ void		lidt(addr_t);
__inline__ void		ltr(uint16_t);
__inline__ int8_t	bsr(uint32_t);
__inline__ int8_t	bsf(uint32_t);

#endif /* !__UTILS_H__ */
