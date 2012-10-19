#include "syscall.h"
	
	.global _sc_hook

_sc_hook:
	pushal

	pushl	%eax
	movw	$0x10,	%ax		# set the system data segment selectors
	movw	%ax,	%ds
	movw	%ax,	%es
	movw	%ax,	%fs
	movw	%ax,	%gs
	popl	%eax
	
	movl	%esp,	%ebp
	
	subl	$0xc,	%esp
	movl	%edx,	0x8(%esp, 1)	# 3rd argument: eip3
	movl	%ecx,	0x4(%esp, 1)	# 2nd argument: esp3
	movl	%eax,	(%esp, 1)	# 1st argument: service id
	call	_sc_enter
	addl	$0xc,	%esp		# restore the kernel stack frame
	movl	%eax,	0x1c(%esp)	# modify the value of EAX to return
					# the system-call error code 

	movw	$0x23,	%ax		# set the user data segment selectors
	movw	%ax,	%ds
	movw	%ax,	%es
	movw	%ax,	%fs
	movw	%ax,	%gs	

	popal				# pop all the GPRs, with ecx and edx
					# filled with the esp3 and eip3
	
	sti
	sysexit
