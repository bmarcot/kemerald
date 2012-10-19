#ifndef __SERIAL_H__
#define __SERIAL_H__

#define SERIAL_N	128

int32_t		serial_init(void);
int32_t		serial_generate(void);
int32_t		serial_free(uint32_t);

#endif /* !__SERIAL_H__ */
