#ifndef __MSR_H__
#define __MSR_H__

#define MSR_IA32_TIME_STAMP_COUNTER	0x10
#define MSR_IA32_APIC_BASE		0x1b
#define MSR_IA32_MPERF			0xe7
#define MSR_IA32_APERF			0xe8
#define MSR_IA32_MISC_ENABLE		0x1a0
#define MSR_IA32_SYSENTER_CS		0x174
#define MSR_IA32_SYSENTER_ESP		0x175
#define MSR_IA32_SYSENTER_EIP		0x176
#define MSR_IA32_THERM_STATUS		0x19c
#define MSR_IA32_CR_PAT			0x277

/* #define MSR_IA32_PLATFORM_ID		0x17 */

/* #define MSR_IA32_PERF_GLOBAL_STATUS	0x38e */

#define MSR_IA32_APIC_BASE_ENABLE	(1 << 11)

#define RDMSR(VALUE, ID)					\
  __asm__ __volatile__ ("	rdmsr			\n\t"	\
			"	movl	%%eax, %0"		\
			: "=m" (*VALUE)				\
			: "c" (ID)				\
			: "%eax", "%edx"			\
			);

#define WRMSR(VALUE, ID)					\
  __asm__ __volatile__ ("	movl	%0, %%eax	\n\t"	\
			"	xorl	%%edx, %%edx	\n\t"	\
			"	wrmsr"				\
			:					\
			: "m" (VALUE), "c" (ID)			\
			: "%eax", "%edx"			\
			);

#define RDMSR2( _id_, _hi_, _lo_)				\
  __asm__ __volatile__ ("	rdmsr"				\
			: "=d" (*_hi_), "=a" (*_lo_)		\
			: "c" (_id_)				\
			);

#define WRMSR2( _id_, _hi_, _lo_)				\
  __asm__ __volatile__ ("	wrmsr"				\
			:					\
			:  "c" (_id_), "d" (_hi_), "a" (_lo_)	\
			);

#define RDMSR3( _id_, _value_)					\
  __asm__ __volatile__ ("	rdmsr"				\
			: "=A" (*_value_)			\
			: "c" (_id_)				\
			);

#define WRMSR3( _id_, _value_)					\
  __asm__ __volatile__ ("	wrmsr"				\
			:					\
			:  "c" (_id_), "A" (_value_)		\
			);


/**
 * Forward declarations
 *
 */

__inline__ uint64_t	rdmsr(uint32_t);
__inline__ void		wrmsr(uint32_t, uint64_t);


#endif /* !__MSR_H__ */

/* __inline__ uint64_t	rdmsr(uint32_t	msr_id) */
/* { */
/*   uint64_t		msr_value; */

/*   __asm__ __volatile__ ("	rdmsr" */
/* 			: "=A" (msr_value) */
/* 			: "c" (msr_id) */
/* 			); */

/*   return msr_value; */
/* } */

/* __inline__ void		wrmsr(uint32_t	msr_id, */
/* 			      uint64_t	msr_value) */
/* { */
/*   __asm__ __volatile__ ("	wrmsr" */
/* 			: */
/* 			: "c" (msr_id), "A" (msr_value) */
/* 			); */
/* } */
