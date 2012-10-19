#ifndef __APIC_LOCAL_H__
#define __APIC_LOCAL_H__

/**
 * Defines
 *
 */

#define	APIC_LOCAL_REG_OFFSET_ID		0x20
#define	APIC_LOCAL_REG_OFFSET_VERSION		0x30
#define	APIC_LOCAL_REG_OFFSET_TASK_PRIORITY	0x80
#define	APIC_LOCAL_REG_OFFSET_PROC_PRIORITY	0xa0
#define	APIC_LOCAL_REG_OFFSET_EOI		0xb0
#define APIC_LOCAL_REG_OFFSET_SVR		0xf0
#define APIC_LOCAL_REG_OFFSET_ISR		0x100
#define APIC_LOCAL_REG_OFFSET_IRR		0x200
#define APIC_LOCAL_REG_OFFSET_LVT_TIMER		0x320
#define APIC_LOCAL_REG_OFFSET_INIT_COUNT	0x380
#define APIC_LOCAL_REG_OFFSET_CUR_COUNT		0x390
#define APIC_LOCAL_REG_OFFSET_DIVIDE_CONFIG	0x3e0

#define APIC_LOCAL_REG_FLAG_SVR_ENABLE		(1 << 8)
#define APIC_LOCAL_REG_FLAG_LVT_SEND_PENDING	(1 << 12)
#define APIC_LOCAL_REG_FLAG_LVT_MASKED		(1 << 16)
#define APIC_LOCAL_REG_FLAG_LVT_TIMER_PERIODIC	(1 << 17)

#define APIC_LOCAL_DIVIDE_CONFIG_1X		11
#define APIC_LOCAL_DIVIDE_CONFIG_2X		0
#define APIC_LOCAL_DIVIDE_CONFIG_4X		1
#define APIC_LOCAL_DIVIDE_CONFIG_8X		2
#define APIC_LOCAL_DIVIDE_CONFIG_16X		3
#define APIC_LOCAL_DIVIDE_CONFIG_32X		8
#define APIC_LOCAL_DIVIDE_CONFIG_64X		9
#define APIC_LOCAL_DIVIDE_CONFIG_128X		10


/**
 * Forward declarations
 *
 */

int32_t			apic_local_init(void);
int32_t			apic_local_enable(bool);
int32_t			apic_local_timer_config(uint32_t);
void			apic_local_ack(void);
int32_t			apic_local_set_task_priority(uint8_t);
uint8_t			apic_local_get_task_priority(void);
uint8_t			apic_local_get_proc_priority(void);
uint8_t			apic_local_is_pending(void);
static uint32_t		_apic_local_rdreg(uint32_t);
static void		_apic_local_wrreg(uint32_t, uint32_t);
static uint8_t		_apic_local_get_id(void);
static void		_apic_local_set_id(uint8_t);
static uint8_t		_apic_local_get_version(void);
static uint8_t		_apic_local_get_max_lvt_entry(void);

#endif /* !__APIC_LOCAL_H__ */
