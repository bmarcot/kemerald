#ifndef __ISR_STAGE2_H__
#define __ISR_STAGE2_H__

int32_t		isr_stage2_init(void);
void		isr_stage2_bind(uint32_t, uint8_t);
static void	_isr_stage2_unhandled(uint32_t);

#endif /* !__ISR_STAGE2_H__ */
