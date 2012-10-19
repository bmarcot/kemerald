/**
 * Mandatory includes
 *
 */
#include "types.h"
#include "kernel.h"
#include "mmu.h"

/**
 * Additional includes
 *
 */
#include "stdlib.h"
#include "stdio.h"
#include "utils.h"

#include "asm.h"

static uint32_t _mmu_phys_bitmap[MMU_MAX_PAGE_N / 32] __attribute__ ((section (".phys_bitmap")));

/* Must be 32-byte aligned. */
static uint64_t	_mmu_pdpt[4] /*__attribute__ ((aligned(0x20)));*/ __attribute__ ((section (".pdptr")));

static mem_block_s	**_blocks;

/**
 * XXX: Mettre en non-cachable les zones I/O ! eg: APIC, ...
 *
 */


/**
 * malloc():
 *
 */
void		*malloc(uint32_t	size)
{
  mem_block_s	*block = *_blocks;
  mem_block_s	*new_block;

  /* If no more free blocks, exit. */
  if (block == NULL)
    {
      return NULL;
    }

  /* Find a free block with enough size. */
  while (block->used || (block->size < size))
    {
      if (block->next != NULL)
	{
	  block = block->next;
	}
      else
	{
	  return NULL;
	}
    }

  /* Set the block as non-free. */
  block->used = true;

  /* Shall the block be splitted ? */
  if (size + sizeof (mem_block_s) + MEM_BLOCK_SIZE_MIN < block->size)
    {
      /* Setup a new free block... */
      new_block = (void *)(block) + sizeof (mem_block_s) + size;
      new_block->used = false;
      new_block->size = block->size - size - sizeof (mem_block_s);
      new_block->prev = block;
      new_block->next = block->next;

      /* ...and reconfigure the allocated block. */
      block->size = size;
      block->next = new_block;
    }

  /* Return the address of the free block + the descriptor's structure. */
  return (void *)(block) + sizeof (struct _mem_block_s);
}


/**
 * free():
 *
 */
int32_t		free(void	*addr)
{
  mem_block_s	*block = *_blocks;
  
  /* Argument checking. */
  if (addr == NULL)
    {
      return ERR_NULLPTR;
    }
  
  /* Check if free blocks remain. */
  if (block == NULL)
    {
      return ERR_UNKNOWN;
    }
  
  /* Find the block to free. */
  while ((void *)(block) + sizeof (mem_block_s) != addr)
    {
      if (block->next != NULL)
	{
	  block = block->next;
	}
      else
	{
	  return NULL;
	}
    }
  
  /* Set the block as free. */
  block->used = false;

  /* If the next block is also free, merge them. */
  if (block->next && (block->next->used == false))
    {
      if (block->next->next)
	{
	  block->next->next->prev = block;
	}
      block->next = block->next->next;
      block->size = block->size + block->next->size + sizeof (mem_block_s);
    }
  
  return ERR_NONE;
}

int32_t		mem_dump(void)
{
  uint32_t	i = 0;
  mem_block_s	*block = *_blocks;

  if (block == NULL)
    return ERR_NULLPTR;
  
  printf("Memory allocation dump:\n");
  while (block)
    {
      printf("\tBlock_%d:\tused=%d, size=%dB,\taddr=%x\n",
	     i++,
	     block->used,
	     block->size,
	     (void *)(block)); 
      block = block->next;
    }
  printf("%d blocks allocated.\n", i);

  return ERR_NONE;
}


/* ------------------------------------------------------------------------ */
/* -------------------------------- PAGING -------------------------------- */
/* ------------------------------------------------------------------------ */

/**
 * mmu_enabled():
 *
 */
bool			mmu_enabled(void)
{
  uint32_t		cr;

  /* Read the CR0 register. */
  cr = rdcr0();

  /* Check if the PG flag is set. */
  if (!(cr & MMU_CR0_FLAG_PG))
    {
      return false;
    }

  /* Read the CR4 register. */
  cr = rdcr4();

  /* Check if the PAE flag is set. */
  if (!(cr & MMU_CR4_FLAG_PAE))
    {
      return false;
    }

  return true;
}


/**
 * _mmu_phys_page_reserve():
 *
 */
static int32_t		_mmu_phys_page_reserve(paddr_t	paddr)
{
  uint32_t		int_index;
  uint32_t		bit_index;
  uint32_t		carry;

  /* Retrieve from the address the int's index into the bitmap. */
  int_index = ((uint32_t) paddr >> 17) & 0x7fff;

  /* Retrieve from the address the bit's offset into the byte. */
  bit_index = ((uint32_t) paddr >> 12) & 0x1f;

  /* XXX: A cause du bug BTS... */
  if ((_mmu_phys_bitmap[int_index] >> bit_index) & 0x1)
    {
      return MMU_ERR_PAGE_NOT_FREE;
    }

  /* Reserve the page. Reservation is atomic. */
  BTS(_mmu_phys_bitmap[int_index], bit_index);

  /* Check if the reservation was successful. */
  RDCARRY(&carry);

  /* If not, then return an non-free page error. */
/*   if (carry) */
/*     { */
/*       return MMU_ERR_PAGE_NOT_FREE; */
/*     } */

  return ERR_NONE;
}


/**
 * mmu_phys_page_free():
 *
 */
static int32_t		_mmu_phys_page_free(paddr_t	paddr)
{
  uint32_t		int_index;
  uint32_t		bit_index;
  uint32_t		carry;

  /* Retrieve from the address the int's index into the bitmap. */
  int_index = ((uint32_t) paddr >> 17) & 0x7fff;

  /* Retrieve from the address the bit's offset into the byte. */
  bit_index = ((uint32_t) paddr >> 12) & 0x1f;

  /* Free the page. Free is atomic. */
  BTR(_mmu_phys_bitmap[int_index], bit_index);

  /* Check if the free was successful. */
  RDCARRY(&carry);

  /* If not, then return an already-freed page error. */
/*   if (!carry) */
/*     { */
/*       return MMU_ERR_PAGE_NOT_FREE; */
/*     } */

  return ERR_NONE;
}


/**
 * _mmu_phys_page_fff():
 *
 */
static paddr_t		_mmu_phys_page_fff(paddr_t	base)
{
  uint32_t	i;
  uint32_t	int_index;
  uint32_t	bit_index;

  /* Walk the physical page bitmap. */
  for (i = (uint32_t) base; i < MMU_PHYS_HEAP_LIMIT; i = i + 0x1000)
    {
      //printf("Try to allocate page @=%x\n", i);
      /* Retrieve from the address the int's index into the bitmap. */
      int_index = (i >> 17) & 0x7fff;

      /* Retrieve from the address the bit's offset into the byte. */
      bit_index = (i >> 12) & 0x1f;

      /* Check if the physical page is free. */
      if (!((_mmu_phys_bitmap[int_index] >> bit_index) & 0x1))
	{
	  /* A free page has been found, return its address. */
	  return (paddr_t) i;
	}
    }

  /* No free page were found, then stop the system... */
  FATAL_ERROR(MMU_MODULE);

  return NULL;
}


/**
 * _mmu_phys_page_alloc():
 *
 */
static paddr_t		_mmu_phys_page_alloc(void)
{
  paddr_t		paddr;

  /* XXX: Laisser les premières pages 0x0 > 0x40000000 en identity-mapping. */
  /* Find the first free physical page. */
  if ((paddr = _mmu_phys_page_fff((paddr_t) MMU_PHYS_HEAP_BASE)) == NULL)
    {
      FATAL_ERROR(MMU_MODULE);
      //return NULL;
    }

  /* Reserve this page. */
  if (_mmu_phys_page_reserve((paddr_t) paddr) != ERR_NONE)
    {
      FATAL_ERROR(MMU_MODULE);
      //return NULL;
    }
  
  /* Return the physical address allocated. */
  return paddr;
}


/**
 * _mmu_phys_clear_bitmap():
 *
 */
static int32_t		_mmu_phys_clear_bitmap(void)
{
  uint32_t		i;

  /* Clear the bitmap. This operation must be protected. */
  for (i = 0; i < MMU_MAX_PAGE_N / 32; i++)
    {
      _mmu_phys_bitmap[i] = 0;
    }

  return ERR_NONE;
}


/**
 * _mmu_page_map():
 *
 */
static int32_t		_mmu_page_map(vaddr_t		vaddr,
				      paddr_t		paddr,
				      uint16_t		opts)
{
  addr_t		pd;
  addr_t		pt;
  pte_t			pte = 0;

/*   if ((uint32_t) vaddr > 0xe0000000) */
/*     { */
/*       printf("page_map() MMU=%d\n", mmu_enabled()); */
/*       //INFINITE_LOOP(); */
/*     } */

  /**
   * If MMU is enabled, then get the page-directory virtual address with the
   * help of mirroring. Else MMU is disabled, thus access the page-directory
   * with its physical address.
   */
  /* Get the physical or virtual address of the page-directory. */
  MMU_GET_PD_ADDR(vaddr, _mmu_pdpt, pd);

  /* If no page-directory entry exists for this address, create a page-table. */
  if (!(((pde_t *) pd)[MMU_GET_PD_INDEX(vaddr)] & MMU_PAGE_OPT_PRESENT))
    {

 /*      if ((uint32_t) vaddr == 0xe0000000) */
/* 	{ */
/* 	  printf("Need a new pt\n"); */
/* 	  // INFINITE_LOOP(); */
/* 	} */

      /* Create a new page-table. */
      if ((pt = mmu_pt_create(vaddr, opts & ~MMU_PAGE_OPT_IDENTITY)) == NULL)
	{
	  return ERR_UNKNOWN;
	}

 /*    if ((uint32_t) vaddr == 0xe0000000) */
/* 	{ */
/* 	  printf(" new pt OK\n"); */
/* 	  //INFINITE_LOOP(); */
/* 	} */
    }

  /* Get the physical or virtual address of the page-table. */
  MMU_GET_PT_ADDR(vaddr, pd, pt);

  /* Build the page-table entry for the requested virtual address. */
  pte = (uint32_t) paddr | MMU_PAGE_OPT_PRESENT | opts;

  /* Insert into the page-table an entry pointing to the page containing the requested address. */
  ((pte_t *) pt)[MMU_GET_PT_INDEX(vaddr)] = pte;

  return ERR_NONE;
}


/**
 * _mmu_page_unmap():
 *
 * MMU: ON.
 */
static int32_t		_mmu_page_unmap(vaddr_t		base)
{
  vaddr_t		pt;

  /* Retrieve the page-table page base address from the virtual address. */
  pt = MMU_GET_PT_VADDR(base);

  /* Assert a page-table entry for the page base address. */
  if (!(((pte_t *) pt)[MMU_GET_PT_INDEX(base)] & MMU_PAGE_OPT_PRESENT))
    {
      return ERR_UNKNOWN;
    }

  /* Unmap the page in the page-table. */
  ((pte_t *) pt)[MMU_GET_PT_INDEX(base)] &= ~((uint64_t) MMU_PAGE_OPT_PRESENT);

  /* XXX: dégager les entrée de la page du TLB !!! */

  return ERR_NONE;
}


/**
 * _mmu_virt_page_fff():
 *
 * Find the first free page into the virtual address-space, starting from the
 * address in argument.
 *
 * MMU: ON/OFF.
 */
static vaddr_t		_mmu_virt_page_fff(vaddr_t	vaddr)
{
  uint16_t		i;
  uint16_t		j;
  addr_t		pd;
  addr_t		pt;

 /*  if (((uint32_t) vaddr > 0xe0000000)) */
/*     { */
/*       if (mmu_enabled() == false) */
/* 	{ */
/* 	  printf(" MMU APS ENABKLE !!!\n"); */
/* 	  FATAL_ERROR(MMU_MODULE); */
/* 	} */
/*       else */
/* 	{ */
/* 	  printf("MMUL enables ok \n"); */
/* 	} */
/*     } */

  /* Get the physical/virtual address of the page-directory. */
  MMU_GET_PD_ADDR(vaddr, _mmu_pdpt, pd);

 /*  if (((uint32_t) vaddr >= 0xe0000000) && ((uint32_t) vaddr < 0xf0000000)) */
/*     printf("mm_virt_page_fff(%x) PD@=%x mmu=%d\n", */
/* 	   (uint32_t) vaddr, (uint32_t) pd, */
/* 	   mmu_enabled() ? 0 : 1); */

  /* Walk the page-directory. */
  for (i = MMU_GET_PD_INDEX(vaddr); i < MMU_PDE_N - 2; i++)
    {
      /* If no page-table exists for the requested virtual address, then return this address. */
      if (!(((pde_t *) pd)[i] & MMU_PAGE_OPT_PRESENT))
	{
	/*   if (((uint32_t) vaddr >= 0xe0000000) && ((uint32_t) vaddr < 0xf0000000)) */
/* 	    printf("MARK 1\n"); */
	  return vaddr;
	}

      /* Get the corresponding physical/virtual address of the page-table. */
      MMU_GET_PT_ADDR(vaddr, pd, pt);

      /* Walk this page-table. */
      for (j = MMU_GET_PT_INDEX(vaddr); j < MMU_PTE_N - i; j++)
	{
	  /* If a free page-table entry is found, then return this base address. */
	  if (!(((pte_t *) pt)[j] & MMU_PAGE_OPT_PRESENT))
	    {
	      return vaddr;
	    }

	  /* Increment by one 4kb page. */
	  vaddr += MMU_PAGE_SIZE;
	}
    }

  /* No virtual page can be found. */
  FATAL_ERROR(MMU_MODULE);
  return NULL;
}


/**
 * _mmu_virt_page_alloc():
 *
 */
static vaddr_t		_mmu_virt_page_alloc(uint16_t	opts)
{
  paddr_t		paddr;
  vaddr_t		vaddr;

  /* Check the options. */
  if (opts & MMU_PAGE_OPT_IDENTITY)
    {
      //return NULL;
      FATAL_ERROR(MMU_MODULE);
    }

  /* Allocate a physical page. */
  paddr = _mmu_phys_page_alloc();

  /* Find room into the page-table. */
  vaddr = _mmu_virt_page_fff(0);

  /* Map the physical page to the virtual page. */
  if (_mmu_page_map(vaddr, paddr, opts) != ERR_NONE)
    {
      FATAL_ERROR(MMU_MODULE);
      //return NULL;
    }

  /* Return the page virtual address. */
  return vaddr;
}


/**
 * mmu_virt_page_reserve():
 *
 * MMU: ON/OFF.
 */
int32_t			mmu_virt_page_reserve(vaddr_t	vaddr,
					      uint16_t	opts)
{
  paddr_t		paddr = vaddr;

/*   printf("MARK II\n"); */

 /*  if ((uint32_t) vaddr > 0xe0000000) */
/*     { */
/*       printf("mm_virt_page_reserve(%x)\n", (uint32_t) vaddr); */
/*       // INFINITE_LOOP(); */
/*     } */

  /* Check if the virtual page is free. */
  if (_mmu_virt_page_fff(vaddr) != vaddr)
    {
      return ERR_UNKNOWN;
    }

 /*  if ((uint32_t) vaddr > 0xe0000000) */
/*     { */
/*       printf("page fff ok\n"); */
/*       // INFINITE_LOOP(); */
/*     } */

/*   printf("page fff ok\n"); */

  /* If the page must be identity-mapped with its physical page. */
  if (opts & MMU_PAGE_OPT_IDENTITY)
    {
     /*  if ((uint32_t) vaddr == 0xe0000000) */
/* 	{ */
/* 	  printf("mm_phys_reserve(%x)\n", (uint32_t) vaddr); */
/* 	  //INFINITE_LOOP(); */
/* 	} */

      /* Check if identity-mapping can be achieved. */
      if (_mmu_phys_page_reserve(vaddr) != ERR_NONE)
	{
	  return ERR_UNKNOWN;
	}

    }
  else
    {
      /* Get the first free physical page. */
      if ((paddr = _mmu_phys_page_alloc()) == NULL)
	{
	  FATAL_ERROR(MMU_MODULE);
	  //return ERR_UNKNOWN;
	}
    }

/*   if ((uint32_t) vaddr == 0xe0000000) */
/*     { */
/*       printf("before page_map()\n"); */
/*       //INFINITE_LOOP(); */
/*     } */

  /* Map the physical page with the virtual page. */
  return _mmu_page_map(vaddr, paddr, opts);
}


/**
 * mmu_virt_page_free():
 *
 * MMU: ON/OFF ?.
 */
int32_t			mmu_virt_page_free(vaddr_t	vaddr)
{
  return ERR_NONE;
}


/**
 * mmu_pd_create():
 *
 * If calling this function with MMU enabled, the page-directory will remain
 * unmapped in the kernel virtual address-space. For further memory access to
 * this page-directory physical page, the physical page shall be mapped to a
 * virtual address inside the kernel (ring-0) address-space. (OR MIRRORING ???)
 */
paddr_t			mmu_pd_create(uint16_t		opts)
{
  uint16_t		i;
  paddr_t		pd_phys;
  vaddr_t		pd_virt;
  pde_t			pde_mirror = 0;

  /* Allocate a physical page which will contain the page-directory. */
  pd_phys = _mmu_phys_page_alloc();

  /* Build the mirroring-entry (present, rw, supervisor, write-back, cache enable). */
  pde_mirror = (uint32_t) pd_phys | MMU_PAGE_OPT_PRESENT | opts;

  /**
   * If the MMU is enabled, then make a temporary mapping of the page-directory
   * physical page into the kernel virtual (logical) address-space. Else, the MMU
   * is disabled and the page-directory physical page is accessible directly with
   * the physical address.
   */
  if (mmu_enabled() == true)
    {
      /* Find a free page into the kernel-shared page-directory. */
      pd_virt = _mmu_virt_page_fff((vaddr_t)(MMU_PDPT_INDEX_RING0_SHARED << 30));

      /* Map the physical page into the kernel virtual address-space. */
      if (_mmu_page_map(pd_virt, pd_phys, MMU_PAGE_OPT_READWRITE) != ERR_NONE)
	{
	  FATAL_ERROR(MMU_MODULE);
	  //return NULL;
	}

      /* Initialize the page-directory with non-present page-table descriptors. */
      for (i = 0; i < MMU_PDE_N - 1; i++)
	{
	  ((pde_t *) pd_virt)[i] = 0ull;
	}

      /* Insert in this directory the mirroring-entry. */
      ((pde_t *) pd_virt)[MMU_PDE_N - 1] = pde_mirror;

      /* Unmap of the kernel address-space, the page-directory temporary mapped page. */
      if (_mmu_page_unmap(pd_virt) != ERR_NONE)
	{
	  FATAL_ERROR(MMU_MODULE);
	  //return NULL;
	}
    }
  else
    {
      /* Initialize the page-directory with non-present page-table descriptors. */
      for (i = 0; i < MMU_PDE_N - 1; i++)
	{
	  ((pde_t *) pd_phys)[i] = 0ull;
	}

      /* Insert in this directory the mirroring-entry. */
      ((pde_t *) pd_phys)[MMU_PDE_N - 1] = pde_mirror;

/*       printf("PDM = %x\n", (uint32_t)(((pde_t *) pd_phys)[MMU_PDE_N - 1])); */
    }

  /* Return the physical address of the page-directory. */
  return pd_phys;
}


/**
 * mmu_pt_create():
 *
 */
paddr_t			mmu_pt_create(vaddr_t		base,
				      uint16_t		opts)
{
  paddr_t		pt_phys;
  addr_t		pt;
  addr_t		pd;
  uint16_t		i;
  pde_t			pde = 0;

 /*  printf("MARK III\n"); */

  /* Set the requested address to a page-table base address. */
  base = (vaddr_t)((uint32_t) base & ~0xfff);

  /* Allocate a physical page which will contain the page-table. */
  pt_phys = _mmu_phys_page_alloc();

  /* Build the PD entry for this PT (present, rw, supervisor, write-back, cache enable). */
  pde = (uint32_t) pt_phys | MMU_PAGE_OPT_PRESENT | opts;

  /* Get the physical/virtual address of the page-directory though mirroring if MMU is enabled. */
  MMU_GET_PD_ADDR(base, _mmu_pdpt, pd);

  /* Insert an entry pointing to this page-table, into the page-directory. */
  ((pde_t *) pd)[MMU_GET_PD_INDEX(base)] = pde;

  /* If MMU is enabled, access the page-table through the mirroring. */
  MMU_GET_PT_ADDR(base, pd, pt);

 /*  printf("Write in PT (@=%x) pd=%x pde=%x pdm=%x\n", (uint32_t) pt, (uint32_t) pd, pde, */
/* 	 (uint32_t)(((pde_t *) pd)[MMU_PDE_N - 1])); */

/*   printf("Print PD\n"); */
/*   for (i = 0; i < MMU_PDE_N; i++) */
/*     { */
/*       if ((uint32_t)(((pde_t *) pd)[i])) */
/* 	printf("PDE=%x VADDR=%x\n", (uint32_t)(((pde_t *) pd)[i]), 0xc0000000 + i * (1 << 21)); */
/*     } */

  /* Initialize the page-table with non-present page-table descriptors. */
  for (i = 0; i < MMU_PDE_N - 1; i++)
    {
      ((pte_t *) pt)[i] = 0ull;
    }

  /**
   * If MMU is enabled, then get the page-directory virtual address with the
   * help of mirroring. Else MMU is disabled, thus access the page-directory
   * with its physical address.
   */
/*   if (mmu_enabled() == true) */
/*     { */
/*       /\* Find a free page into the kernel-shared page-directory. *\/ */
/*       pt_virt = _mmu_virt_page_fff((vaddr_t)(MMU_PDPT_INDEX_RING0_SHARED << 30)); */
/*       //pt_virt = (void *) 0xd0000000; */

/*       /\* Map the physical page into the kernel virtual address-space. *\/ */
/*       // XXX: PASSER PAR LE MIRRORING !!! */
/*       if (_mmu_page_map(pt_virt, pt_phys, MMU_PAGE_OPT_READWRITE) != ERR_NONE) */
/* 	{ */
/* 	  //return NULL; */
/* 	  FATAL_ERROR(MMU_MODULE); */
/* 	} */

/*       /\* Initialize the page-directory with non-present page-table descriptors. *\/ */
/*       for (i = 0; i < MMU_PDE_N - 1; i++) */
/* 	{ */
/* 	  ((pte_t *) pt_virt)[i] = 0ull; */
/* 	} */

/*       /\* Unmap of the kernel address-space, the page-directory temporary mapped page. *\/ */
/*       if (mmu_page_unmap(pt_virt) != ERR_NONE) */
/* 	{ */
/* 	  //return NULL; */
/* 	  FATAL_ERROR(MMU_MODULE); */
/* 	} */
/*     } */
/*   else */
/*     { */
/*       /\* Initialize the page-table with non-present page-descriptors. *\/ */
/*       for (i = 0; i < MMU_PTE_N - 1; i++) */
/* 	{ */
/* 	  ((pte_t *) pt_phys)[i] = 0ull; */
/* 	} */
/*     } */

  /* Return the physical address of the page-table. */
  return pt_phys;
}


/**
 * mmu_init():
 *
 */
int32_t		mmu_init(void)
{
  paddr_t	pd;
  uint32_t	i;
  uint64_t	pd_ptr;

  /* Build the two page-directory for the address range from 0 to 0x7fffffff. */
  printf("Build the PDs\n");
  for (i = 0; i < 4; i++)
    {
      pd = mmu_pd_create(MMU_PAGE_OPT_READWRITE | MMU_PAGE_OPT_WRITETROUGH);
      pd_ptr = ((uint32_t) pd & ~0xfff) | MMU_PAGE_OPT_PRESENT;
      _mmu_pdpt[i] = pd_ptr;
    }
  //INFINITE_LOOP();

  /* Map in identity-mapping the address range from 0 to 0x3fffffff. */
  printf("Achieve the identity-mapping\n");
  for (i = 0; i < MMU_PHYS_HEAP_BASE; i = i + 0x1000)
    {
      if (mmu_virt_page_reserve((vaddr_t) i,
				MMU_PAGE_OPT_READWRITE | MMU_PAGE_OPT_USER | MMU_PAGE_OPT_IDENTITY)
	  != ERR_NONE)
	{
	  printf("A memory page reservation failed...\n");
	  INFINITE_LOOP();
	}
    }

  /* XXX: A degager dans LAPIC_INIT() et IOAPIC_INIT(). */

  /* Reserve the Local APIC address map. */
/*   if (mmu_virt_page_reserve((vaddr_t) 0xfee00000, */
/* 			    MMU_PAGE_OPT_READWRITE | MMU_PAGE_OPT_WRITETROUGH | MMU_PAGE_OPT_CACHE_DISABLE | MMU_PAGE_OPT_IDENTITY)) */
/*     { */
/*       printf("An io page reservation failed...\n"); */
/*       INFINITE_LOOP(); */
/*     } */

/*   /\* Reserve the IO APIC address map. *\/ */
/*   if (mmu_virt_page_reserve((vaddr_t) 0xfec00000, */
/* 			    MMU_PAGE_OPT_READWRITE | MMU_PAGE_OPT_WRITETROUGH | MMU_PAGE_OPT_CACHE_DISABLE | MMU_PAGE_OPT_IDENTITY)) */
/*     { */
/*       printf("An io page reservation failed...\n"); */
/*       INFINITE_LOOP(); */
/*     } */

  /* XXX: ! */


/*   for (i = 0xfe000000; i <= 0xfef00000; i = i + 0x1000) */
/*     { */
/*       if (mmu_virt_page_reserve((vaddr_t) i, */
/* 				MMU_PAGE_OPT_READWRITE | MMU_PAGE_OPT_WRITETROUGH | MMU_PAGE_OPT_CACHE_DISABLE | MMU_PAGE_OPT_IDENTITY)) */
/* 	{ */
/* 	  printf("An io page reservation failed...\n"); */
/* 	  INFINITE_LOOP(); */
/* 	} */
/*     } */

  printf("Load the PDBR with PDPT=%x\n", (uint32_t) _mmu_pdpt);
  /* Load the PDBR with the page-directory pointer table base address. */
  MMU_LOAD_PDBR(_mmu_pdpt);
 
  printf("Enable the paging\n");
  /* Enable the PAE feature. */
  MMU_ENABLE_PAE();

  /* Enable the MMU translation. */
  MMU_ENABLE_PAGING();

  *_blocks = (void *)(MEM_HEAP_START);
  (*_blocks)->used = false;
  (*_blocks)->size = MEM_HEAP_END - MEM_HEAP_START - sizeof (mem_block_s);
  (*_blocks)->prev = NULL;
  (*_blocks)->next = NULL;

  return ERR_NONE;
}
