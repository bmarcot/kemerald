#ifndef __EX_H__
#define __EX_H__

#define EX_DIVIDE_ERROR			0
#define EX_DOUBLE_FAULT			8
#define EX_GENERAL_PROTECTION		13
#define EX_PAGE_FAULT			14
#define EX_ERR_CODE_PF_PAGELEVEL	(1 << 0)
#define EX_ERR_CODE_PF_WRITEACCESS	(1 << 1)
#define EX_ERR_CODE_PF_RING3		(1 << 2)


int32_t		ex_init(void);
static void	_ex_divide_error(uint32_t);
static void	_ex_double_fault(uint32_t);
static void	_ex_general_protection(uint32_t);
static void	_ex_page_fault(uint32_t);


#endif /* !__EX_H__ */
