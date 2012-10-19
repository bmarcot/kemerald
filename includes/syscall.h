#ifndef __SC_H__
#define __SC_H__

#define SC_N		9

#define SC_SYSENTER2(SERVICE_ID)					\
  __asm__ __volatile__ ("	pushl	%%ebp			\n\t"	\
			"	movl	%%esp, %%ebp		\n\t"	\
			"	movl	$rfsc, %%edx		\n\t"	\
			"	movl	%%esp, %%ecx		\n\t"	\
			"	sysenter			\n\t"	\
			"rfsc:					\n\t"	\
			"	popl	%%ebp"				\
			:						\
			: "a" (SERVICE_ID)				\
			: "%ecx", "%edx"				\
			);

#define SC_SYSENTER(SERVICE_ID, STATUS)					\
  __asm__ __volatile__ ("	movl	$rfsc, %%edx		\n\t"	\
			"	movl	%%esp, %%ecx		\n\t"	\
			"	sysenter			\n\t"	\
			"rfsc:					\n\t"	\
			"	movl	%%eax, %0"			\
			: "=a" (*STATUS)				\
			: "a" (SERVICE_ID)				\
			: "%ecx", "%edx"				\
			);

extern void	sc_hook(void);

int32_t		sc_init();
int32_t		sc_sysenter(uint32_t);
int32_t		sc_enter(uint32_t, uint32_t, uint32_t);

int32_t		sc_thread_stop(uint32_t);
int32_t		sc_sleep(uint32_t);

#endif /* !__SC_H__ */
