#include "types.h"

int32_t	memset(void	*dest,
	       char	value,
	       uint32_t	count)
{
  __asm__ ("0:				\n\t"
	   "	testl	%2, %2		\n\t"
	   "	jz	1f		\n\t"
	   "	movb	%1, (%0)	\n\t"
	   "	incl	%0		\n\t"
	   "	decl	%2		\n\t"
	   "	jmp	0b		\n\t"
	   "1:				"
	   :
	   : "D" (dest), "S" (value), "c" (count)
	   );

  return 0;
}


int32_t	memcpy(void	*dest,
	       void	*src,
	       uint32_t	count)
{
  __asm__ ("0:				\n\t"
	   "	testl	%2, %2		\n\t"
	   "	jz	1f		\n\t"
	   "	movb	(%1), %%al	\n\t"
	   "	movb	%%al, (%0)	\n\t"
	   "	incl	%0		\n\t"
	   "	incl	%1		\n\t"
	   "	decl	%2		\n\t"
	   "	jmp	0b		\n\t"
	   "1:				"
	   :
	   : "D" (dest), "S" (src), "c" (count)
	   : "%eax");

  return 0;
}


int32_t	memzero(void	*dest,
		uint32_t	count)
{
  __asm__ ("0:				\n\t"
	   "	testl	%1, %1		\n\t"
	   "	jz	1f		\n\t"
	   "	movb	$0, (%0)	\n\t"
	   "	incl	%0		\n\t"
	   "	decl	%1		\n\t"
	   "	jmp	0b		\n\t"
	   "1:				"
	   :
	   : "D" (dest), "c" (count)
	   );

  return 0;
}
