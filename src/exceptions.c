/**
 * Mandatory includes
 *
 */
#include "types.h"
#include "kernel.h"
#include "exceptions.h"

/**
 * Additional includes
 *
 */
#include "isr_stage2.h"
#include "utils.h"

/* XXX: a enlever en release... */
#include "console.h"

/**
 * ex_init():
 *
 */
int32_t			ex_init(void)
{
  /* Bind the divide error handler. */
  isr_stage2_bind((uint32_t) _ex_divide_error, EX_DIVIDE_ERROR);

  /* Bind the double fault handler. */
  isr_stage2_bind((uint32_t) _ex_double_fault, EX_DOUBLE_FAULT);

  /* Bind the general protection fault handler. */
  isr_stage2_bind((uint32_t) _ex_general_protection, EX_GENERAL_PROTECTION);

  /* Bind the page fault handler. */
  isr_stage2_bind((uint32_t) _ex_page_fault, EX_PAGE_FAULT);

  return ERR_NONE;
}


/**
 * _ex_divide_error():
 *
 */
static void		_ex_divide_error(uint32_t	err_code)
{
  printf("#DE - Divide Error\n", 0);

  /* Kill and restart the task. */
  /* XXX: no action at the moment, freeze the sys... */
  while (1)
    hlt();
}


/**
 * _ex_double_fault():
 *
 */
static void		_ex_double_fault(uint32_t	err_code)
{
  console_printf("#DF\n", BG_RED | FG_WHITE | FG_INTENSITY);
  printf("#DF - Double Fault (%x)\n", err_code);

  /* Kill and restart the task. */
  /* XXX: no action at the moment, freeze the sys... */
  while (1)
    hlt();
}


/**
 * _ex_general_protection():
 *
 */
static void		_ex_general_protection(uint32_t	err_code)
{
  console_printf("#GP\n", BG_RED | FG_WHITE | FG_INTENSITY);
  printf("#GP - General Protection (%x)\n", err_code);

  /* Kill and restart the task. */
  /* XXX: no action at the moment, freeze the sys... */
  while (1)
    hlt();
}


/**
 * _ex_page_fault():
 *
 */
static void		_ex_page_fault(uint32_t		err_code)
{
 uint32_t		faulty_address;

  console_printf("#PF\n", BG_RED | FG_WHITE | FG_INTENSITY);

  /* retrieve the faulty address from CR2 register */
  faulty_address = rdcr2();
  printf("\tFaulty address:\t\t%x\n", faulty_address);

  /* get the error conditions */
  printf("\tFaulty event:\t\t");
  if (err_code & EX_ERR_CODE_PF_PAGELEVEL)
    printf("page level violation\n");
  else
    printf("non-present page\n");

  printf("\tFaulty access type:\t");
  if (err_code & EX_ERR_CODE_PF_WRITEACCESS)
    printf("write\n");
  else
    printf("read\n");

  printf("\tFaulty access mode:\t");
  if (err_code & EX_ERR_CODE_PF_RING3)
    printf("user\n");
  else
    printf("supervisor\n");

  /* XXX: XD bit? */

  /* XXX: If triggered by a thread, kill it! */
 
  /* Kill and restart the task. */
  /* XXX: no action at the moment, freeze the sys... */
  while (1)
    hlt();
}
