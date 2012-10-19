#ifndef __PORT_H__
#define __PORT_H__

/* 8259A */
#define PORT_PIC_MASTER_COMMAND		0x20
#define PORT_PIC_MASTER_DATA		0x21
#define PORT_PIC_SLAVE_COMMAND		0xa0
#define PORT_PIC_SLAVE_DATA		0xa1

/* PIT */
#define PORT_PIT_COMMAND		0x43
#define PORT_PIT_DATA			0x40

/* Keyboard controller */
#define PORT_KEYBOARD_COMMAND		0x64
#define PORT_KEYBOARD_DATA		0x60


#define OUTB(_value_, _port_)					\
  __asm__ __volatile__("	outb	%0, %1"			\
		       :					\
		       : "a" (_value_), "Nd" (_port_)		\
		       );

#define OUTW(_value_, _port_)					\
  __asm__ __volatile__("	outw	%0, %1"			\
		       :					\
		       : "a" (_value_), "Nd" (_port_)		\
		       );

#define OUTL(_value_, _port_)					\
  __asm__ __volatile__("	outl	%0, %1"			\
		       :					\
		       : "a" (_value_), "Nd" (_port_)		\
		       );

#define INB(_value_, _port_)					\
  __asm__ __volatile__ ("	inb	%1, %0"			\
			: "=a" (*_value_)			\
			: "Nd" (_port_)				\
			);

#define INW(_value_, _port_)					\
  __asm__ __volatile__ ("	inw	%1, %0"			\
			: "=a" (*_value_)			\
			: "Nd" (_port_)				\
			);

#define INL(_value_, _port_)					\
  __asm__ __volatile__ ("	inl	%1, %0"			\
			: "=a" (*_value_)			\
			: "Nd" (_port_)				\
			);



#define INW_VPORT(VALUE, PORT)					\
  __asm__ __volatile__ ("	in	%%dx, %%ax"		\
			: "=a" (*VALUE)				\
			: "d" (PORT)				\
			);

#define INB_VPORT(VALUE, PORT)					\
  __asm__ __volatile__ ("	in	%%dx, %%al"		\
			: "=a" (*VALUE)				\
			: "d" (PORT)				\
			);


__inline__ void		outb(uint16_t, uint8_t);
__inline__ void		outw(uint16_t, uint16_t);
__inline__ void		outl(uint16_t, uint32_t);
__inline__ uint8_t	inb(uint16_t);
__inline__ uint16_t	inw(uint16_t);
__inline__ uint32_t	inl(uint16_t);

#endif /* !__PORT_H__ */
