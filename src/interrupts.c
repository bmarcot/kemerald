/**
 * Mandatory includes
 *
 */
#include "types.h"
#include "kernel.h"
#include "interrupts.h"

/**
 * Additional includes
 *
 */
#include "isr_stage2.h"
#include "apic_local.h"
#include "scheduler.h"
#include "port.h"

/* XXX: a enlever en release... */
#include "console.h"

static uint32_t		_apic_local_timer_n = 0;
static uint32_t		_apic_io_timer_n = 0;


/**
 * int_init():
 *
 */
int32_t			int_init(void)
{
  /* Bind the local-apic timer handler. */
  isr_stage2_bind((uint32_t) _int_apic_local_timer, INT_APIC_LOCAL_TIMER);

  /* Bind the io-apic timer handler. */
  isr_stage2_bind((uint32_t) _int_apic_io_timer, INT_APIC_IO_TIMER);

  /* Bind the io-apic second timer handler. */
  isr_stage2_bind((uint32_t) _int_apic_io_timer2, INT_APIC_IO_TIMER2);

  /* Bind the io-apic keyboard handler. */
  isr_stage2_bind((uint32_t) _int_apic_io_keyboard, INT_APIC_IO_KEYBOARD);

  return ERR_NONE;
}


/**
 * _int_apic_local_timer():
 *
 */
static void		_int_apic_local_timer(void/* uint32_t		err_code */)
{
  /* uint32_t esp, ebp; */

  //console_printf("#APIC Timer\n", BG_BLUE | FG_CYAN | FG_INTENSITY);

  /* RDESP(&esp); */
  /*   RDEBP(&ebp); */
  //printf("-APIC timer- (%d) esp:%x ebp:%x\n", _apic_local_timer_n++, esp, ebp);
  printf("__APIC timer__ (%d) :: Proc P = %d\n",
	 _apic_local_timer_n++,
	 apic_local_get_proc_priority());
 
  if (_apic_local_timer_n == 10)
    {
      if (thread_stop(2) == ERR_UNKNOWN)
	printf(">> Error on thread_stop...\n");
      else
	printf(">> thread_stop()\n");
    }
  if (_apic_local_timer_n == 20)
    {
      if (thread_start(2) == ERR_UNKNOWN)
	printf(">> Error on thread_start...\n");
      else
	printf(">> thread_start()\n");
    }

#ifdef __PRINT_MSG_LAPIC_TIMER
  fprintf(stdout, "(Local-APIC)\tTimer interrupt: %d\n", _apic_local_timer_n++);
#endif /* !__PRINT_MSG_LAPIC_TIMER */
  
  //  printf("Apic timer end\n");

  /* acknowledge the timer interrupt */
  //apic_local_ack();

  sched_elect();


}


/**
 * _int_apic_io_timer():
 *
 */
static void		_int_apic_io_timer(uint32_t	err_code)
{
  //console_printf("#IO-APIC Timer\n", BG_BLUE | FG_CYAN | FG_INTENSITY);

  //tty_refresh();

#ifdef __PRINT_MSG_IOAPIC_TIMER
  if (_apic_io_timer_n && !(_apic_io_timer_n % 300))
    fprintf(stdout, "(IO-APIC)\tPIT interrupt: %r\n", _apic_io_timer_n);
#endif /* !__PRINT_MSG_IOAPIC_TIMER */

  /* increment the counter */
  _apic_io_timer_n++;

  /* acknowledge the timer interrupt */
  apic_local_ack();
}


/**
 * _int_apic_io_timer2():
 *
 */
static void		_int_apic_io_timer2(uint32_t	err_code)
{
  printf("Pit IRQ:2\n", 0);

  apic_local_ack();
}


/**
 * _int_apic_io_keyboard():
 *
 */
static void		_int_apic_io_keyboard(void/* uint32_t	err_code */)
{
  uint8_t		scancode;
  
  console_printf("#KB\n", BG_GREEN | FG_WHITE | FG_INTENSITY);

  /* XXX: scancode = inb() */
  //INB(&scancode, PORT_KEYBOARD_DATA);
  scancode = inb(PORT_KEYBOARD_DATA);

  /* If the top bit of the byte we read from the keyboard is
   *  set, that means that a key has just been released */
  if (scancode & 0x80)
    {
      /* You can use this one to see if the user released the
       *  shift, alt, or control keys... */
      printf("(IO-APIC)\t\tKeyboard interrupt (key released): %x\n", scancode);
    }
  else
    {
      printf("(IO-APIC)\t\tKeyboard interrupt: %x :: Proc P = %d\n",
	     scancode,
	     apic_local_get_proc_priority());
    }

  /* acknowledge the timer interrupt */
  //  apic_local_ack();
}
