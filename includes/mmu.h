#ifndef __MMU_H__
#define __MMU_H__

/**
 * Defines
 *
 */

#define MMU_MODULE			"MMU"

#define MEM_HEAP_START			0x1000000
#define MEM_HEAP_END			0x2000000
#define MEM_BLOCK_SIZE_MIN		4

#define MMU_MAX_PAGE_N			(1 << 20)
#define MMU_PDE_N			(1 << 9)
#define MMU_PTE_N			(1 << 9)
#define MMU_ERR_BASE			-100
#define MMU_ERR_NOT_CONTIGUOUS		(MMU_ERR_BASE - 1)
#define MMU_ERR_PAGE_NOT_FREE		(MMU_ERR_BASE - 2)
#define MMU_PAGE_SIZE			0x1000
#define MMU_MIRROR			0x1ff
#define MMU_CR0_FLAG_PG			(1 << 31)
#define MMU_CR4_FLAG_PAE		(1 << 5)

#define MMU_PHYS_HEAP_BASE		0x4000000
#define MMU_PHYS_HEAP_LIMIT		0x8000000

#define MMU_PAGE_OPT_NONE		0
#define MMU_PAGE_OPT_PRESENT		(1 << 0)
#define MMU_PAGE_OPT_READWRITE		(1 << 1)
#define MMU_PAGE_OPT_USER		(1 << 2)
#define MMU_PAGE_OPT_WRITETROUGH	(1 << 3)
#define MMU_PAGE_OPT_CACHE_DISABLE	(1 << 4)
#define MMU_PAGE_OPT_PAT		(1 << 7)
#define MMU_PAGE_OPT_GLOBAL		(1 << 8)
#define MMU_PAGE_OPT_IDENTITY		(1 << 9)
#define MMU_PAGE_OPT_STRONG_UNCACHEABLE	(MMU_PAGE_OPT_WRITETROUGH	\
					 | MMU_PAGE_OPT_CACHE_DISABLE	\
					 | MMU_PAGE_OPT_PAT)
#define MMU_PAGE_OPT_WRITEBACK		MMU_PAGE_OPT_PAT
//#define MMU_PAGE_OPT_MIRROR

#define MMU_PDPT_INDEX_RING3_CODE	0
#define MMU_PDPT_INDEX_RING3_DATA	1
#define MMU_PDPT_INDEX_RING0_STACK	2
#define MMU_PDPT_INDEX_RING0_SHARED	3


/**
 * Macros
 *
 */

#define MMU_LOAD_PDBR(ADDR)						\
  __asm__ __volatile__ ("	movl	%0, %%cr3"			\
			:						\
			: "r" (ADDR)					\
			);

#define MMU_ENABLE_PAGING()						\
  __asm__ __volatile__ ("	movl	%%cr0, %%eax		\n\t"	\
			"	orl	$0x80000000, %%eax	\n\t"	\
			"	movl	%%eax, %%cr0"			\
			:						\
			:						\
			: "%eax"					\
			);

#define MMU_ENABLE_PAE()						\
  __asm__ __volatile__ ("	movl	%%cr4, %%eax		\n\t"	\
			"	orl	$0x20, %%eax		\n\t"	\
			"	movl	%%eax, %%cr4"			\
			:						\
			:						\
			: "%eax"					\
			);

#define MMU_MAKE_ADDR(_dir_, _hi_, _lo_, _offset_)			\
  ((((_dir_) & 0x3) << 30)						\
   | (((_hi_) & 0x1ff) << 21)						\
   | (((_lo_) & 0x1ff) << 12)						\
   | (_offset_))

#define MMU_GET_PD_INDEX(_base_)					\
  (((uint32_t) _base_ >> 21) & 0x1ff)

#define MMU_GET_PT_INDEX(_base_)					\
  (((uint32_t) _base_ >> 12) & 0x1ff)

#define MMU_GET_PD_VADDR(_vaddr_)					\
  ((vaddr_t) MMU_MAKE_ADDR(((uint32_t) _vaddr_ >> 30) & 0x3, MMU_MIRROR, MMU_MIRROR, 0))

#define MMU_GET_PT_VADDR(_vaddr_)					\
  ((vaddr_t) MMU_MAKE_ADDR(((uint32_t) _vaddr_ >> 30) & 0x3, MMU_MIRROR, MMU_GET_PD_INDEX(_vaddr_), 0))

#define MMU_GET_PD_ADDR(_vaddr_, _pdpt_, _pd_)				\
  if (mmu_enabled() == true)						\
    _pd_ = (void *) MMU_GET_PD_VADDR(_vaddr_);				\
  else									\
    _pd_ = ((void *)((uint32_t) _pdpt_[((uint32_t) _vaddr_ >> 30) & 0x3] & 0xfffff000));

#define MMU_GET_PT_ADDR(_vaddr_, _pd_, _pt_)				\
  if (mmu_enabled() == true)						\
    _pt_ = (void *) MMU_GET_PT_VADDR(_vaddr_);				\
  else									\
    _pt_ = ((void *)((uint32_t)((pde_t *) _pd_)[MMU_GET_PD_INDEX(_vaddr_)] & 0xfffff000));


/**
 * Types
 *
 */

struct			_mem_block_s
{
  uint32_t		used;
  uint32_t		size;
  struct _mem_block_s	*prev;
  struct _mem_block_s	*next;
} __attribute__ ((packed));
typedef struct		_mem_block_s mem_block_s;
		
typedef uint64_t	pte_t;
typedef uint64_t	pde_t;


/**
 * Forward declarations
 *
 */

int32_t			mmu_init(void);
int32_t			mem_dump(void);
void			*malloc(uint32_t);
int32_t			free(void *);

int32_t			mmu_init(void);
bool			mmu_enabled(void);
static int32_t		_mmu_phys_page_reserve(paddr_t);
static int32_t		_mmu_phys_page_free(paddr_t);
static paddr_t		_mmu_phys_page_fff(paddr_t);
static paddr_t		_mmu_phys_page_alloc(void);
static int32_t		_mmu_phys_clear_bitmap(void);
static vaddr_t		_mmu_virt_page_fff(vaddr_t);
vaddr_t			mmu_virt_page_alloc(uint16_t);
int32_t			mmu_virt_page_reserve(vaddr_t, uint16_t);
paddr_t			mmu_pd_create(uint16_t);
paddr_t			mmu_pt_create(vaddr_t, uint16_t);
static int32_t		_mmu_page_map(vaddr_t, paddr_t, uint16_t);
static int32_t		_mmu_page_unmap(vaddr_t);

int32_t			mmu_test(void);

#endif /* !__MMU_H__ */
