#ifndef __IOAPIC_H__
#define __IOAPIC_H__

#define IOAPIC

int32_t	ioapic_read_register(uint32_t *,uint32_t);
int32_t	ioapic_write_register(uint32_t,uint32_t);

#endif /* !__IOAPIC_H__ */
