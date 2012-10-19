/**
 * Mandatory includes
 *
 */
#include "types.h"
#include "kernel.h"
#include "segment.h"

/**
 * Additional includes
 *
 */
#include "utils.h"


/* GDT descriptors must be qword-aligned (8 bytes alignement) */
static uint64_t		_seg_gdt[SEG_DESC_N] __attribute__ ((aligned(8))); // auto alignée ?
static uint32_t		_seg_tss[SEG_TSS_FIELD_N];


/**
 * seg_init():
 *
 */
int32_t			seg_init(void)
{
  uint8_t		i;

  /* Init the GDT entries to null value, including the null entry. */
  for (i = 0; i < SEG_DESC_N; i++)
    {
      _seg_gdt[i] = 0ull;
    }

  /* Init the TSS fields to null. */
  for (i = 0; i < SEG_TSS_FIELD_N; i++)
    {
      _seg_tss[i] = 0;
    }

  /* Add the ring 0 code segment descriptor. */
  _seg_gdt_add_descriptor(SEG_GDT_INDEX_RING0_CODE, 0, 0xfffff,
			  SEG_DESC_FLAG_GRANULARITY_4KB
			  | SEG_DESC_FLAG_32BITS
			  | SEG_DESC_FLAG_PRESENT
			  | SEG_DESC_FLAG_NON_SYSTEM
			  | SEG_DESC_FIELD_TYPE_CODE_RX);

  /* Add the ring 0 data segment descriptor. */
  _seg_gdt_add_descriptor(SEG_GDT_INDEX_RING0_DATA, 0, 0xfffff,
			  SEG_DESC_FLAG_GRANULARITY_4KB
			  | SEG_DESC_FLAG_32BITS
			  | SEG_DESC_FLAG_PRESENT
			  | SEG_DESC_FLAG_NON_SYSTEM
			  | SEG_DESC_FIELD_TYPE_DATA_RW);

  /* Add the ring 3 code segment descriptor. */
  _seg_gdt_add_descriptor(SEG_GDT_INDEX_RING3_CODE, 0, 0xfffff,
			  SEG_DESC_FLAG_GRANULARITY_4KB
			  | SEG_DESC_FLAG_32BITS
			  | SEG_DESC_FLAG_PRESENT
			  | SEG_DESC_FLAG_NON_SYSTEM
			  | SEG_DESC_FIELD_TYPE_CODE_RX
			  | SEG_DESC_FIELD_DPL_RING3);

  /* Add the ring 3 data segment descriptor. */
  _seg_gdt_add_descriptor(SEG_GDT_INDEX_RING3_DATA, 0, 0xfffff,
			  SEG_DESC_FLAG_GRANULARITY_4KB
			  | SEG_DESC_FLAG_32BITS
			  | SEG_DESC_FLAG_PRESENT
			  | SEG_DESC_FLAG_NON_SYSTEM
			  | SEG_DESC_FIELD_TYPE_DATA_RW
			  | SEG_DESC_FIELD_DPL_RING3);

  /* Add the TSS descriptor. */
  _seg_gdt_add_descriptor(SEG_GDT_INDEX_TSS,
			  (uint32_t) _seg_tss,
			  SEG_TSS_FIELD_N * sizeof (uint32_t) - 1,
			  SEG_DESC_FLAG_PRESENT
			  | SEG_DESC_FIELD_TYPE_TSS_BUSY);

  /* Load the GDTR with a new selector. */
  _seg_gdt_load_selector();

  /* Change the flow of execution, and serializes the processor. */
  _seg_cpu_serialize();

  /* Configure some mandatory fields into the TSS. */
  _seg_tss[SEG_TSS_FIELD_OFFSET_SS0] = SEG_SELECTOR_RING0_DATA;

  /* Load the Task Register with the TSS selector. */
  ltr(SEG_SELECTOR_TASK_REGISTER);

  return ERR_NONE;
}


/**
 * _seg_gdt_add_descriptor():
 *
 */
static void		_seg_gdt_add_descriptor(uint8_t		index,
						uint32_t	base,
						uint32_t	limit,
						uint32_t	attribs)
{
  /* Reset the GDT entry. */
  _seg_gdt[index] = 0ull;

  /* Configure the segment's limit. */
  _seg_gdt[index] |= ((limit & MASK_16BITS) | (((uint64_t) limit & 0xf0000) << 32));

  /* Configure the segment's base. */
  _seg_gdt[index] |= ((((uint64_t) base & MASK_16BITS) << 16)
		      | (((uint64_t) base & 0xff0000) << 16)
		      | (((uint64_t) base & 0xff000000) << 32));

  /* Configure the attributes for this segment descriptor. */
  _seg_gdt[index] |= ((uint64_t) attribs << 32);
}


/**
 * _seg_gdt_load_selector():
 *
 */
static void		_seg_gdt_load_selector(void)
{
  seg_gdtr_s		gdtr;

  /* Hardbuild the selector. */
  gdtr.limit = SEG_DESC_N * sizeof (uint64_t);
  gdtr.base = (uint32_t) _seg_gdt;

  /* Load the GDT register with this selector. */
  lgdt(&gdtr);
}


/**
 * _seg_cpu_serialize():
 *
 */
static __inline__ void	_seg_cpu_serialize(void)
{
  SEG_CPU_SERIALIZE();
}


/**
 * seg_tss_set_esp0():
 *
 */
void			seg_tss_set_esp0(uint32_t	esp0)
{
  _seg_tss[SEG_TSS_FIELD_OFFSET_ESP0] = esp0;
}
