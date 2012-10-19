#ifndef __APIC_IO_H__
#define __APIC_IO_H__

/**
 * Defines
 *
 */

#define APIC_IO_BASE			0xfec00000

#define	APIC_IO_REG_INDEX		APIC_IO_BASE
#define	APIC_IO_REG_DATA		(APIC_IO_BASE + 0x10)
#define	APIC_IO_REG_EOI			(APIC_IO_BASE + 0x40)

#define	APIC_IO_REG_OFFSET_ID		0x0
#define	APIC_IO_REG_OFFSET_VERSION	0x1
/* auto-generated on Mon Jul 26 14:16:14     2010 */
#define APIC_IO_REG_OFFSET_REDTBL_0	16
#define APIC_IO_REG_OFFSET_REDTBL_1	18
#define APIC_IO_REG_OFFSET_REDTBL_2	20
#define APIC_IO_REG_OFFSET_REDTBL_3	22
#define APIC_IO_REG_OFFSET_REDTBL_4	24
#define APIC_IO_REG_OFFSET_REDTBL_5	26
#define APIC_IO_REG_OFFSET_REDTBL_6	28
#define APIC_IO_REG_OFFSET_REDTBL_7	30
#define APIC_IO_REG_OFFSET_REDTBL_8	32
#define APIC_IO_REG_OFFSET_REDTBL_9	34
#define APIC_IO_REG_OFFSET_REDTBL_10	36
#define APIC_IO_REG_OFFSET_REDTBL_11	38
#define APIC_IO_REG_OFFSET_REDTBL_12	40
#define APIC_IO_REG_OFFSET_REDTBL_13	42
#define APIC_IO_REG_OFFSET_REDTBL_14	44
#define APIC_IO_REG_OFFSET_REDTBL_15	46
#define APIC_IO_REG_OFFSET_REDTBL_16	48
#define APIC_IO_REG_OFFSET_REDTBL_17	50
#define APIC_IO_REG_OFFSET_REDTBL_18	52
#define APIC_IO_REG_OFFSET_REDTBL_19	54
#define APIC_IO_REG_OFFSET_REDTBL_20	56
#define APIC_IO_REG_OFFSET_REDTBL_21	58
#define APIC_IO_REG_OFFSET_REDTBL_22	60
#define APIC_IO_REG_OFFSET_REDTBL_23	62
/* !auto-generated */

#define APIC_IO_REG_FLAG_REDTBL_MASKED	(1 << 16)


/**
 * Forward declarations
 *
 */

int32_t			apic_io_init(void);
/* void			apic_io_ack(void); */
static uint32_t		_apic_io_rdreg(uint32_t);
static void		_apic_io_wrreg(uint32_t, uint32_t);
static uint8_t		_apic_io_get_id(void);
static uint32_t		_apic_io_get_version(void);
static void		_apic_io_add_redentry(uint8_t, uint8_t);

#endif /* !__APIC_IO_H__ */
