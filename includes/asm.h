#ifndef __ASM_H__
#define __ASM_H__

/* #define RDREG(VALUE, ADDR)					\ */
/*   __asm__ __volatile__ ("	movl	(%1), %%eax	\n\t"	\ */
/* 			"	movl	%%eax, %0"		\ */
/* 			: "=m" (*VALUE)				\ */
/* 			: "r" (ADDR)				\ */
/* 			: "%eax"				\ */
/* 			); */

#define RDREG(VALUE, ADDR)					\
  __asm__ __volatile__ ("	movl	(%1), %0"		\
			: "=r" (*VALUE)				\
			: "r" (ADDR)				\
			);

/* #define WRREG(VALUE, ADDR)					\ */
/*   __asm__ __volatile__ ("	movl	%0, %%eax	\n\t"	\ */
/* 			"	movl	%%eax, (%1)"		\ */
/* 			:					\ */
/* 			: "m" (VALUE), "r" (ADDR)		\ */
/* 			: "%eax"				\ */
/* 			); */

#define WRREG(VALUE, ADDR)					\
  __asm__ __volatile__ ("	movl	%0, (%1)"		\
			:					\
			: "r" (VALUE), "r" (ADDR)		\
			);

/* #define CLI()					\ */
/*   __asm__ __volatile__ ("cli"); */

/* #define STI()					\ */
/*   __asm__ __volatile__ ("sti"); */

#define HLT()					\
  __asm__ __volatile__ ("hlt");

#define RDESP(VALUE)						\
  __asm__ __volatile__ ("	movl	%%esp, %0"		\
			: "=r" (*VALUE)				\
			);

#define RDEBP(VALUE)						\
  __asm__ __volatile__ ("	movl	%%ebp, %0"		\
			: "=r" (*VALUE)				\
			);

/* #define RDCARRY(VALUE)						\ */
/*   __asm__ __volatile__ ("	movl	$0, %0		\n\t"	\ */
/* 			"	jnc	0f		\n\t"	\ */
/* 			"	movl	$1, %0		\n\t"	\ */
/* 			"0:"					\ */
/* 			: "=r" (*VALUE)				\ */
/* 			); */
#define RDCARRY(VALUE)						\
  *VALUE = 0;

#define BT(BIT_BASE, BIT_INDEX)				\
  __asm__ __volatile__ ("	btl	%1, %0"			\
			: "=m" (BIT_BASE)			\
			: "r" (BIT_INDEX)			\
			);

/* #define BTS(BIT_BASE, BIT_INDEX)				\ */
/*   __asm__ __volatile__ ("	btl	%1, %0		\n\t"	\ */
/* 			"	jc	0f		\n\t"	\ */
/* 			"lock	btsl	%1, %0		\n\t"	\ */
/* 			"0:"					\ */
/* 			: "=r" (BIT_BASE)			\ */
/* 			: "r" (BIT_INDEX)			\ */
/* 			); */
#define BTS(BIT_BASE, BIT_INDEX)				\
  BIT_BASE |= (1 << BIT_INDEX);

#define BTS2(BIT_BASE, BIT_OFFSET)				\
  __asm__ __volatile__ ("	btsl	%1, %0"			\
			: "=r" (BIT_BASE)			\
			: "r" (BIT_OFFSET)			\
			);

/* #define BTR(BIT_BASE, BIT_INDEX)				\ */
/*   __asm__ __volatile__ ("	btl	%1, %0		\n\t"	\ */
/* 			"	jnc	0f		\n\t"	\ */
/* 			"lock	btrl	%1, %0		\n\t"	\ */
/* 			"0:"					\ */
/* 			: "=r" (BIT_BASE)			\ */
/* 			: "r" (BIT_INDEX)			\ */
/* 			); */
#define BTR(BIT_BASE, BIT_INDEX)				\
  BIT_BASE &= ~(1 << BIT_INDEX);


#define CPUID(VALUE, EAX, EBX, ECX, EDX)				\
  __asm__ __volatile__ ("	cpuid			\n\t"		\
			"	movl	%%eax, %0	\n\t"		\
			"	movl	%%ebx, %1	\n\t"		\
			"	movl	%%ecx, %2	\n\t"		\
			"	movl	%%edx, %3"			\
			: "=m" (*EAX), "=m" (*EBX), "=m" (*ECX), "=m" (*EDX) \
			: "a" (VALUE)					\
			: "%ebx", "%ecx", "%edx"			\
			);


/**
 * Forward declarations
 *
 */


#endif /* !__ASM_H__ */
