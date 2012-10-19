#ifndef __INT_H__
#define __INT_H__

#define INT_APIC_LOCAL_TIMER		32
#define INT_APIC_IO_KEYBOARD		33
#define INT_APIC_IO_TIMER		34
#define INT_APIC_IO_TIMER2		35


int32_t		int_init(void);
static void	_int_apic_local_timer(void/* uint32_t */);
static void	_int_apic_io_timer(uint32_t);
static void	_int_apic_io_timer2(uint32_t);
static void	_int_apic_io_keyboard(void/* uint32_t */);

#endif /* !__INT_H__ */
