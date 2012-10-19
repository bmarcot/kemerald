#ifndef __KERNEL_H__
#define __KERNEL_H__

/**
 * Defines
 *
 */

#define BASE_OCT	8
#define BASE_DEC	10
#define BASE_HEX	16

#define ERR_NONE	0
#define ERR_UNKNOWN	-1
#define ERR_NULLPTR	-2


/**
 * Macros
 *
 */

#define INFINITE_LOOP()				\
  while (1)					\
    HLT();

#define FATAL_ERROR(__name__)				\
  printf("Fatal error in module ");			\
  printf(__name__);					\
  printf(" at line %d... System halted.\n", __LINE__);	\
  while (1)						\
    HLT();


/**
 * Forward declarations
 *
 */

void	_main(void) __attribute__ ((section (".entry"), noreturn));

int32_t	test_multiline(void);
int32_t	test_itoa(void);
int32_t	test_println(void);
int32_t	test_splashscreen(void);
int32_t	test_msr(void);
int32_t	test_apic(void);
int32_t	test_interrupts(void);
int32_t	test_ide_dma(void);

//int32_t	mmu_build_pd(void);

void	pci_list(uint8_t,uint8_t,uint8_t);

#endif /* !__KERNEL_H__ */
