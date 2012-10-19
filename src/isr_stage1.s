
	.globl _isr_stage1
	
_isr_stage1:
	pushal
	pushl	%ds
	pushl	%es
	pushl	%fs
	pushl	%gs
	movw	$0x10,	%ax   # Load the Kernel DS desc! (if ring change!)
	movw	%ax,	%ds
	movw	%ax,	%es
	movw	%ax,	%fs
	movw	%ax,	%gs
	pushl	52(%esp)	# error code
	movl	52(%esp), %eax	# interrupt number
	#pushl	%eax
	movl	$___isr_stage2, %ebx
	call *	(%ebx, %eax, 4)
	#addl	$8,	%esp
	addl	$4,	%esp

	# int probing
	movl	$_isr_stage2_pending, %ebx
	call *	%ebx
	
	popl	%gs
	popl	%fs
	popl	%es
	popl	%ds
	popal
	addl	$8,	%esp # cleans up the pushed data during stage 0
	#sti	#modifier plutot les EFLAGS ici !!
	# Que des interrupt gates donc pas besoin de remettre le flag IF a 1 car
	# il a été sauvegardé sur la pile à 1 lors de l'interruption, et sera donc
	# restauré à 1 lors du IRET (pareil pour le CLI en stage-0 à retirer).
	# XXX: attention utile pour les EXCEPTIONS !
	iret


	.globl	_isr_stage1_ex
_isr_stage1_ex:
	pushal
	pushl	%ds
	pushl	%es
	pushl	%fs
	pushl	%gs
	movw	$0x10,	%ax   # Load the Kernel DS desc! (if ring change!)
	movw	%ax,	%ds
	movw	%ax,	%es
	movw	%ax,	%fs
	movw	%ax,	%gs
	pushl	52(%esp)	# error code
	movl	52(%esp), %eax	# interrupt number
	movl	$___isr_stage2, %ebx
	call *	(%ebx, %eax, 4)
	addl	$4,	%esp

	popl	%gs
	popl	%fs
	popl	%es
	popl	%ds
	popal
	addl	$8,	%esp # cleans up the pushed data during stage 0
	sti	#modifier plutot les EFLAGS ici !!
	iret
	

	.globl	_isr_stage1_int
_isr_stage1_int:
	pushal
	pushl	%ds
	pushl	%es
	pushl	%fs
	pushl	%gs
	movw	$0x10,	%ax   # Load the Kernel DS desc! (if ring change!)
	movw	%ax,	%ds
	movw	%ax,	%es
	movw	%ax,	%fs
	movw	%ax,	%gs
	movl	48(%esp), %eax	# interrupt number

	movl	$___isr_stage2, %ebx
	call *	(%ebx, %eax, 4)

	# int probing
	movl	$_isr_stage2_pending, %ebx
	call *	%ebx
	
	popl	%gs
	popl	%fs
	popl	%es
	popl	%ds
	popal
	addl	$4,	%esp # cleans up the pushed data during stage 0
	# Que des interrupt gates donc pas besoin de remettre le flag IF a 1 car
	# il a été sauvegardé sur la pile à 1 lors de l'interruption, et sera donc
	# restauré à 1 lors du IRET (pareil pour le CLI en stage-0 à retirer).
	iret
