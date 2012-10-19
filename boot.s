.code16
#.code32
	
.org 0x0

.section .text
	
bootloader:
	cli
	nop
	
	# initialisation de CS en mode réel
	ljmp	$0x07c0, $b
b:
	nop
	nop
	# initialisation des segments de données en mode réel
	movw	$0x07c0, %ax
	movw 	%ax,	%ds
	movw 	%ax,	%es
	movw	%ax,	%fs
	movw	%ax,	%gs
	# initialisation de la pile en mode réel (utilité?) 
	movw	%ax, 	%ss
	movw	$0x600,	%sp

start:
	movb	$'R',	%al
	movb	$0xe,	%ah
	movw	$0x7, 	%bx
	int 	$0x10

	/*
#recopie du kernel 32bits

	movw	$0x0000, %ax
	movw 	%ax,	%ds
	leaw	k_start + 0x7c00, %si
	leaw	0x1000, %di
	leaw	k_end + 0x7c00, %cx
loop:	cmpw	%cx, %si
	jz	loop_ok
	movw 	(%si), %ax
	movw	%ax, (%di)
	incw	%si
	incw	%di
	jmp	loop
loop_ok:
	movw	$0x07c0, %ax
	movw 	%ax,	%ds
*/
/*	# DEBUG
toto:	
	jmp	toto
# !DEBUG*/


# recopie du kernel depuis le HD vers la RAM (0x0:0x1000)
	push	%es
	#movw	$0x0100, %ax
	movw	$0x0900, %ax
	movw 	%ax,	%es
	movw	$0x0,	%bx
	movb	$0x2,	%ah
	movb	$0x43,	%al # nombre de blocs chargeables apparament limités
	movb	$0x0,	%ch
	movb	$0x1, 	%cl
	movb	$0x0,	%dh
	movb	$0x80,	%dl
	int 	$0x13
	pop	%es
	
	# Construire le descripteur de la GDT /!\
	
	# Passage en mode protected
	lgdt 	gdt_desc


	
	movl 	%cr0,	%eax
	orl 	$1, 	%eax
	movl 	%eax,	%cr0


	
	# flush des caches du CPU
	jmp	next
next:
	# Recharger DS, ES, FS, GS avec le bon "segment selector"
	# soit le troisieme "segment descriptor" de la GDT.
	movw	$0x10,	%ax
	movw	%ax, 	%ds
	movw	%ax, 	%es
	movw	%ax, 	%fs	
	movw	%ax, 	%gs
	movw	%ax, 	%ss	

	# Initialiser la pile
	movl	$0x4000, %esp
	movl	$0x4000, %ebp

	# Long jump pour recharger le segment selector dans CS
	#ljmp *	$0x8,	$(protected + 0x7c00)
	
	#ljmp 	$0x8,	$(toto + 0x7c00)

toto:	
	#sti
#toto1:
	#jmp toto1
	#sti
	#hlt

	
	ljmp 	$0x8,	$0x9000
	
protected:
	.code32
/*endd:	
	jmp *	(0x7c00 + endd)*/

k_start:
	/*xorl	%eax, %eax
	movl	$0xbebe, %eax*/
	# Ecrire dans le frame buffer video
	movl	$0xb8000, %eax
	movl	$'P',	(%eax)
	incl	%eax
	movl 	$0x1b, 	(%eax)
	hlt
	/*jmp *	0x1000*/
k_end:	
	
	
gdt:
	
gdt_null:
.quad 	0x0

gdt_code:
.word	0xffff
.word	0x0000
.byte 	0x0
.byte	0x9a
.byte	0xcf
.byte	0x0

gdt_data:
.word 	0xffff
.word 	0x0000
.byte	0x0
.byte	0x93
.byte	0xcf
.byte	0x0
	
gdt_end:

gdt_desc:
.word	(gdt_end - gdt)
.long 	(0x7c00 + gdt)
	
# Remplir avec des zero le reste du secteur
.fill 0x1fe - (. - bootloader) , 1, 0
# Ajouter la signature d'un MBR
.long 0xaa55
