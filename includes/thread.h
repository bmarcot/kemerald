#ifndef __THREAD_H__
#define __THREAD_H__

#include "list.h"

#define THREAD_N		3


/**
 * XXX: on devrait pouvoir passer les valeurs EIP et ESP3 sans avoir à
 * recréer des variables automatiques. Bug des macros avec cette version de
 * GCC ? Passer une référence à la mémoire ou passer dans des registres ? A
 * creuser...
 */
/* #define THREAD_START_IN_RING3(EIP, ESP3)				\ */
/*   __asm__ __volatile__ ("	pushl	$0x23		\n\t"		\ */
/* 			"	pushl	%1		\n\t"		\ */
/* 			"	pushfl			\n\t"		\ */
/* 			"	orl	$0x200, (%%esp)	\n\t"		\ */
/* 			"	andl	$0xffffbfff, (%%esp)\n\t"	\ */
/* 			"	pushl	$0x1b		\n\t"		\ */
/* 			"	pushl	%0		\n\t"		\ */
/* 			"	movw	$0x23,	%%ax	\n\t"		\ */
/* 			"	movw	%%ax,	%%ds	\n\t"		\ */
/* 			"	movw	%%ax,	%%es	\n\t"		\ */
/* 			"	movw	%%ax,	%%fs	\n\t"		\ */
/* 			"	movw	%%ax,	%%gs	\n\t"		\ */
/*   			"	iret"					\ */
/* 			:						\ */
/* 			: "r" (EIP), "r" (ESP3)				\ */
/* 			: "%eax"					\ */
/* 			); */
#define THREAD_START_IN_RING3(EIP, ESP3)				\
  __asm__ __volatile__ ("	pushl	$0x23		\n\t"		\
			"	pushl	%1		\n\t"		\
			"	pushfl			\n\t"		\
			"	orl	$0x200, (%%esp)	\n\t"		\
			"	andl	$0xffffbfff, (%%esp)\n\t"	\
			"	pushl	$0x1b		\n\t"		\
			"	pushl	%0		\n\t"		\
			"	movw	$0x23,	%%ax	\n\t"		\
			"	movw	%%ax,	%%ds	\n\t"		\
			"	movw	%%ax,	%%es	\n\t"		\
			"	movw	%%ax,	%%fs	\n\t"		\
			"	movw	%%ax,	%%gs	\n\t"		\
  			"	iret"					\
			:						\
			: "r" (EIP), "r" (ESP3)				\
			);


/**
 * 1) No need to set the ring0 data selectors, beacause we are already in
 *	ring0 and we will saty in ring0.
 * 2) Because there is no privilege change, stack selector and stack pointer
 *	will not be popped from the current stack during the IRET execution.
 *	Therefore they must be set by hand.
 * 3) Push the EFLAGS and set the interrupt enable bit (IE), and clear the
 *	nested task bit (NT).
 * 4) Push the ring0 code selector.
 * 5) Push the kernel-thread entry point.
 */
#define THREAD_START_IN_RING0(EIP, ESP0)				\
  __asm__ __volatile__ ("	movl	%1,	%%esp	\n\t"		\
			"	pushfl			\n\t"		\
			"	orl	$0x200, (%%esp)	\n\t"		\
			"	andl	$0xffffbfff, (%%esp)\n\t"	\
			"	pushl	$0x8		\n\t"		\
			"	pushl	%0		\n\t"		\
			"	iret"					\
			:						\
			: "r" (EIP), "r" (ESP0)				\
			);

/* #define THREAD_SWITCH(PREV_ESP0, NEXT_ESP0)			\ */
/*   __asm__ __volatile__ ("	pushfl			\n\t"	\ */
/* 			"	pushl	%%ebp		\n\t"	\ */
/* 			"	movl	%%esp,  %0	\n\t"	\ */
/* 			"	movl	%1,	%%esp	\n\t"	\ */
/* 			"	pushl	$1f		\n\t"	\ */
/* 			"	ret			\n\t"	\ */
/* 			"1:				\n\t"	\ */
/* 			"	popl	%%ebp		\n\t"	\ */
/* 			"	popfl"				\ */
/* 			:					\ */
/* 			: "m" (PREV_ESP0), "m" (NEXT_ESP0)	\ */
/* 			); */
#define THREAD_SWITCH(PREV_ESP0, NEXT_ESP0)			\
  __asm__ __volatile__ ("	pushfl			\n\t"	\
			"	pushl	%%ebp		\n\t"	\
			"	movl	%%esp,  %0	\n\t"	\
			"	movl	%1,	%%esp	\n\t"	\
			"	popl	%%ebp		\n\t"	\
			"	popfl"				\
			: "=m" (PREV_ESP0)			\
			: "m" (NEXT_ESP0)			\
			);

#define THREAD_SETUP_STACK(PREV_ESP0)				\
  __asm__ __volatile__ ("	pushfl			\n\t"	\
			"	pushl	%%ebp		\n\t"	\
			"	movl	%%esp,	%0"		\
			:					\
			: "m" (PREV_ESP0)			\
			);

enum		_thread_state_e
  {
    DORMANT,
    READY,
    RUNNING,
    WAITING
  };
typedef enum	_thread_state_e thread_state_e;

enum		_thread_class_e
  {
    SYSTEM,
    USER
  };
typedef enum	_thread_class_e thread_class_e;

/* struct		_s_thread_tss */
/* { */
/*   uint32_t	esp0; */
/*   uint32_t	iomap; */
/* } __attribute__((packed)); */
/* typedef struct	_s_thread_tss s_thread_tss; */

typedef struct	__attribute__((packed))	_s_thread_tss
{
  uint32_t	esp0;
  uint32_t	iomap;
} s_thread_tss;


/**
 * This structure defines a 'threadcontext'. The first parameters are
 * hardware-independent (from the microkernel architecture), whereas the
 * second parameters defines hardware-dependent parameters.
 *
 * Hardware-independent parameters :
 *	- uid: a thread's unique identificator
 *	- priority: thread's priority (from 0 to 255, and 0 being the highest
 *		priority)
 *	- name: a thread's name (user-defined)
 *	- state: a thread's state ()
 *
 * Hardware-dependent parameters (for Intel IA-32 architecture):
 *	- eip: address of the entry point (i.e. a function)
 *	- esp0: address of the ring0's stack
 *	- esp3: address of the ring3's stack
 *	- tss_esp0 : address of the base of the ring0 stack
 */
struct			_thread_ctx_s
{
  /* hardware-independent parameters */
  uint32_t		id;
  uint32_t		priority;
  uchar			name[8];
  thread_state_e	state;
  thread_class_e	class;
  uint32_t		entry;

  /* hardware-dependent parameters (Intel IA-32 architecture) */
  uint32_t		eip;
  uint32_t		esp0;
  uint32_t		esp3;
  s_thread_tss		tss;
} /*__attribute__((packed))*/;
typedef struct _thread_ctx_s thread_ctx_s;


int32_t		thread_init(void);
int32_t		thread_create(uint32_t, thread_state_e, char *);
int32_t		thread_switch(thread_ctx_s *, thread_ctx_s *);
void		thread_launch(void);
int32_t		thread_start(uint32_t);
int32_t		thread_stop(uint32_t);
list_s		*thread_get_ready(void);
void		thread_bg(void);

#endif /* !__THREAD_H__ */
