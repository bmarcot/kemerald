#ifndef __SEG_H__
#define __SEG_H__


/* There is at least 6 different descriptors to be put into the SEG :
 *	- the null descriptor
 *	- two ring0 descriptors for code and data
 *	- two ring3 descriptors for code and data
 *	- the TSS descriptor
 */
#define SEG_DESC_N			6
#define SEG_TSS_FIELD_N			26

#define SEG_TSS_FIELD_OFFSET_ESP0	1
#define SEG_TSS_FIELD_OFFSET_SS0	2

#define SEG_DESC_FLAG_GRANULARITY_4KB	(1 << 23)
#define SEG_DESC_FLAG_32BITS		(1 << 22)
#define SEG_DESC_FLAG_PRESENT		(1 << 15)
#define SEG_DESC_FLAG_NON_SYSTEM	(1 << 12) // 1 for code and data, 0 for system (TSS, IDT) 
#define SEG_DESC_FIELD_DPL_RING3	(3 << 13)
#define SEG_DESC_FIELD_TYPE_DATA_RW	(2 << 8)
#define SEG_DESC_FIELD_TYPE_CODE_RX	(0xa << 8)
#define SEG_DESC_FIELD_TYPE_TSS_BUSY	(9 << 8)

#define SEG_GDT_INDEX_NULL		0
#define SEG_GDT_INDEX_RING0_CODE	1
#define SEG_GDT_INDEX_RING0_DATA	2
#define SEG_GDT_INDEX_RING3_CODE	3
#define SEG_GDT_INDEX_RING3_DATA	4
#define SEG_GDT_INDEX_TSS		5

#define SEG_SELECTOR_RPL_RING3		3
#define SEG_SELECTOR_RING0_CODE		(SEG_GDT_INDEX_RING0_CODE << 3)
#define SEG_SELECTOR_RING0_DATA		(SEG_GDT_INDEX_RING0_DATA << 3)
#define SEG_SELECTOR_RING3_CODE		((SEG_GDT_INDEX_RING3_CODE << 3) | SEG_SELECTOR_RPL_RING3)
#define SEG_SELECTOR_RING3_DATA		((SEG_GDT_INDEX_RING3_DATA << 3) | SEG_SELECTOR_RPL_RING3)
#define SEG_SELECTOR_TASK_REGISTER	(SEG_GDT_INDEX_TSS << 3)

#define SEG_CPU_SERIALIZE()					\
  __asm__ __volatile__ ("	ljmp	$0x08,	$0f	\n\t"	\
			"0:");


struct		_seg_gdtr_s
{
  uint16_t	limit;
  uint32_t	base;
} __attribute__ ((packed));
typedef struct _seg_gdtr_s seg_gdtr_s;


int32_t			seg_init(void);
void			seg_tss_set_esp0(uint32_t);
static void		_seg_gdt_add_descriptor(uint8_t, uint32_t, uint32_t, uint32_t);
static void		_seg_gdt_load_selector(void);
static __inline__ void	_seg_cpu_serialize(void);

#endif /* !__SEG_H__ */
