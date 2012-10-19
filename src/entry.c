#include "types.h"
#include "kernel.h"
//#include "gdt.h"
#include "framebuffer.h"
#include "string.h"
#include "stdlib.h"
//#include "idt.h"
//#include "isr.h"

#include "msr.h"
#include "pic.h"
#include "pit.h"
#include "asm.h"
#include "port.h"
#include "console.h"
#include "tty.h"

#include "stdio.h"
#include "stdarg.h"

#include "thread.h"
#include "scheduler.h"

#include "mmu.h"
#include "syscall.h"
#include "serial.h"
#include "video.h"
#include "apic_local.h"
#include "apic_io.h"

#include "segment.h"
//#include "gdt.h"

#include "idt2.h"
#include "exceptions.h"
#include "interrupts.h"
#include "msr.h"
#include "utils.h"

//extern s_gdt_descriptor	gdt[GDT_DESCRIPTOR_N];

//#define _TEST_MSR_
//#define _TEST_APIC_
/* #define _TEST_INTERRUPTS_ */
//#define _TEST_MULTILINE_
//#define _TEST_ITOA_
//#define _TEST_PRINTLN_
//#define _TEST_IDE_DMA_

#define PCI_FG FG_GREEN

//uint32_t	sysenter_stack[128];

//uint32_t i = 54;

void		vmx_supported(void)
{
  uint32_t	eax, ebx, ecx, edx;
  uint32_t	value = 1;

  CPUID(value, &eax, &ebx, &ecx, &edx);

  printf("< CPUID >\n\teax: %x\n\tebx: %x\n\tecx: %x\n\tedx: %x\n",
	 eax, ebx, ecx, edx);

  printf("VMX-operations supported:\t");
  if ((ecx & (1 << 5)) == 1)
    {
      printf("[OK]\n");
    }
  else
    {
      printf("[NO]\n");
    }

  printf("SYSENTER and SYSEXIT supported:\t");
  if ((edx & (1 << 11)) == 1)
    {
      printf("[OK]\n");
    }
  else
    {
      printf("[NO]\n");
    }

  value = 1;
  CPUID(value, &eax, &ebx, &ecx, &edx);
  printf("PAE-ext supported:\t\t");
  if ((edx & (1 << 6)) == 1)
    {
      printf("[OK]\n");
    }
  else
    {
      printf("[NO]\n");
    }

  while (1)
    ;
}

void		test_malloc()
{
  uint32_t	*u, *v, *w, *x;

  /* test the malloc() function */

  u = (uint32_t *) malloc(sizeof (uint32_t));
  *u = 0xdeadbeef;
  
  v = (uint32_t *) malloc(1048 * sizeof (uint32_t));
  *v = 0xfadebabe;
  
  w = (uint32_t *) malloc(sizeof (uint32_t));
  *w = 0xabadcafe;

  printf("malloc() test\n");

  printf("\tpointer @ =\t%x\n", (uint32_t) u);
  printf("\tpointer val =\t%x\n", (uint32_t) *u);

  printf("\tpointer @ =\t%x\n", (uint32_t) v);
  printf("\tpointer val =\t%x\n", (uint32_t) *v);

  printf("\tpointer @ =\t%x\n", (uint32_t) w);
  printf("\tpointer val =\t%x\n", (uint32_t) *w);

  /* test the free() function */

  printf("free() test\n");

  if (free(u) != ERR_NONE)
    {
      printf("free() FAILED...\n");
      while (1)
	HLT();
    }

  x = (uint32_t *) malloc(sizeof (uint32_t));
  *x = 0xfeedf00d;

  if (x != u)
    {
      printf("A free block is not reused...\n");
      while (1)
	HLT();
    }

  printf("\tpointer @ =\t%x\n", (uint32_t) x);
  printf("\tpointer val =\t%x\n", (uint32_t) *x);
}



void		_main(void)
{
  uint32_t i;

  /**
   *
   *	Basic setup
   */

  /* enable the A20 line */
 
  /* disable the PIC (8259A chip) */
  pic_enable(false);

  /* disable the local APIC */
  //apic_enable(false);
  //apic_local_enable(false)

  /**
   *
   *	Protection
   */

  /* init and enable segmentation */
  //gdt_init();

  seg_init();
  //  INFINITE_LOOP();

  /* init and enable paging */
  //mmu_init();
  console_clear();
  mmu_init();

  /* init the system-call facilities */
  sc_init();

  /**
   *
   *	Interrupts
   */
  // INFINITE_LOOP();
  /* init and enable interrupts */
  idt_init();

  //INFINITE_LOOP();

  /* bind stage2 isrs */
  //isr_init();
  int_init();
  ex_init();

  //INFINITE_LOOP();

  /* configure the Local-APIC timer */
  //apic_local_timer_init(0x2ffffff);
  //apic_local_timer_init(0xffffff);

  /* configure the PIT timer */
  pit_init();

  printf("Local-APIC init...\n");
  /* Init the Local-APIC. */
  apic_local_init();
  apic_local_set_task_priority(0);
  printf("Local-APIC timer config...\n");
  apic_local_timer_config(0x2ffffff);
  printf("IO-APIC init...\n");
  apic_io_init();
  /* enable the local APIC */
  //apic_enable(true);
  apic_local_enable(true);

  printf("OK\n");

  /* configure the I/O APIC in charge of the PIT, keyboard, ... */
  //apic_io_init();
  /* STI(); */
/*   INFINITE_LOOP(); */

  /* clear the screen */
  console_clear();
  console_init();

  /* init the basic I/O services */
  stdio_init();
  //tty_init();
  //tty_activate(0);

 /*  sysenter(); */
  /*   vmx_supported(1); */
  test_malloc();

  /* init the serial number facility */
  serial_init();
  /*   printf("UID %d\n", serial_generate()); */
  /*   printf("UID %d\n", serial_generate()); */
  /*   printf("UID %d\n", serial_generate()); */

  /* init the task manager */
  thread_init();

  /* init the scheduler manager */
  sched_init();

  /* dump the memory */
  mem_dump();

  /* test the mmu. */
  //mmu_test();
  /* while (1) */
/*     ; */

  //video_init();

/*   msr = rdmsr(MSR_IA32_APIC_BASE); */
/*   printf("msr hi: %x\n", (uint32_t)(msr >> 32)); */
/*   printf("msr lo: %x\n", (uint32_t) msr); */
/*   msr = rdmsr(MSR_IA32_CR_PAT); */
/*   printf("msr hi: %x\n", (uint32_t)(msr >> 32)); */
/*   printf("msr lo: %x\n", (uint32_t) msr); */
/*   msr = rdmsr(MSR_IA32_PERF_GLOBAL_STATUS); */
/*   printf("msr hi: %x\n", (uint32_t)(msr >> 32)); */
/*   printf("msr lo: %x\n", (uint32_t) msr); */
/*   msr = rdmsr(MSR_IA32_PLATFORM_ID); */
/*   printf("msr hi: %x\n", (uint32_t)(msr >> 32)); */
/*   printf("msr lo: %x\n", (uint32_t) msr); */
/*   printf("Macrotest\n"); */
/*   MACROTEST(&msr); */
/*   printf("msr hi: %x\n", (uint32_t)(msr >> 32)); */
 /*  printf("apic-io ver: %x\n", apic_io_get_version()); */
/*   printf("apic-io id: %x\n", apic_io_get_id()); */

/*   printf("New GDT:\n"); */
/*   seg_init(); */
/*   printf("Old GDT:\n"); */
/*   for (i = 0; i < SEG_DESC_N; i++) */
/*     { */
/*       printf("%x - %x\n", gdt[i].high, gdt[i].low); */
/*     } */
  //INFINITE_LOOP();

  i = 1 << 5 | 1 << 9;
  printf("bsf(i) = %d\n", bsf(i));
  printf("bsr(i) = %d\n", bsr(i));
  i = 0;
  printf("bsr(i) = %d\n", bsr(i));

  printf("MSR_IA32_SYSENTER_CS high = %x\n", (uint32_t)(rdmsr(MSR_IA32_SYSENTER_CS) >> 32));
  printf("MSR_IA32_SYSENTER_CS low = %x\n", (uint32_t) rdmsr(MSR_IA32_SYSENTER_CS));

  printf("IA32_CR_PAT high = %x\n", (uint32_t)(rdmsr(MSR_IA32_CR_PAT) >> 32));
  printf("IA32_CR_PAT low = %x\n", (uint32_t) rdmsr(MSR_IA32_CR_PAT));

  /* jump to background task - code after this function is never reached */
  sched_launch();


  /*   test_multiline(); */
  /*   test_println(); */
  /*   test_interrupts(); */
  /*   test_itoa(); */
  /*   test_msr(); */
  //test_apic();
  //test_ide_dma();

  //printf("+ Bus 0\n", BG_BLACK | FG_RED | FG_INTENSITY);
  //pci_list(0, 2, 64);
  /*   printf("Bus 1\n", BG_BLACK | FG_RED | FG_INTENSITY); */
  /*   pci_list(1); */
  //test_ide_dma();
  //test_apic();

  //fprintf(0, "Protos v%d.%d - id:%x\n", 0, 12, 0xbe01);
  //fprintf(0, "\tid reg:\t%r\n", 0xbe01);
  //fprintf(0, "Ok!\n");
  // printf("Helo %% %d-%x-%r\n", 45, 0xabc78, 0xabc78);
  //printf("Helo %r\n", 0xbe01);

  //r = fprintf(stdout, "Almost %x years :)\n", 26);
  //fprintf(stdout, "size:%d\n", strlen("hello!"));

  //printf("fprintf result = %d\n", r);
  //printf(&((&stdio_filedes[0])->buffer[0]));

  //__asm__ ("rdtsc");


  // Halt the system
  while (1)
    HLT();
}

/* int32_t		test_multiline(void) */
/* { */
/* #ifdef _TEST_MULTILINE_ */
/*   uint32_t_t	i; */
/*   char		str[32]; */
/*   char		buffer[4]; */

/*   for (i = 0; i < 32; i++) */
/*     { */
/*       memzero(str, 32); */
/*       memzero(buffer, 4); */

/*       strcat(str, "line: "); */
/*       itoa(buffer, i, 10); */
/*       strcat(str, buffer); */
/*       strcat(str, " (0x"); */
/*       memzero(buffer, 4); */
/*       itoa(buffer, i, 16); */
/*       strcat(str, buffer); */
/*       strcat(str, ")"); */
/*       fb_println(str, BG_BLACK | FG_WHITE); */
/*     } */
/* #endif /\* !_TEST_MULTILINE_ *\/ */

/*   return 0; */
/* } */

/* int32_t		test_itoa(void) */
/* { */
/* #ifdef _TEST_ITOA_ */
/*   char buffer[32]; */

/*   memzero(buffer, 32); */
/*   itoa(buffer, 456789, 10); */
/*   fb_println(buffer, BG_MAGENTA | FG_GREEN | FG_INTENSITY); */

/*   memzero(buffer, 32); */
/*   itoa(buffer, 4560789, 10); */
/*   fb_println(buffer, BG_BLUE | FG_CYAN); */
/* #endif /\* !_TEST_ITOA_ *\/ */

/*   return 0; */
/* } */

/* void		pci_list(uint8_t	bus, */
/* 			 uint8_t	device_n, */
/* 			 uint8_t	function_n) */
/* { */
/* #ifdef __PCI_LIST__ */
/*   bool		device_found = false; */
/*   uint8_t	device; */
/*   uint8_t	function; */
/*   uint32_t	v; */
/*   char		buffer[32]; */

/*   /\* browse devices *\/ */
/*   for (device = 0; device <= device_n; device++) */
/*     { */
/*       /\* browse function *\/ */
/*       for (function = 0; function <= function_n; function++) */
/* 	{ */
/* 	  v = 0x80000000 | (bus << 16) | (device << 11) | (function << 8); */
/* 	  OUTL(v, 0x0cf8); */
/* 	  INL(&v, 0x0cfc); */
/* 	  if ((v & 0xffff) == 0x8086) */
/* 	    { */
/* 	      if (device_found == false) */
/* 		{ */
/* 		  printf("|- Device: ", BG_BLACK | FG_WHITE | FG_INTENSITY); */
/* 		  memzero(buffer, 32); */
/* 		  itoa(buffer, device, BASE_HEX); */
/* 		  printf(buffer,  BG_BLACK | FG_WHITE | FG_INTENSITY); */
/* 		  printf("\n", 0); */
/* 		  device_found = true; */
/* 		} */
/* 	      printf("\t|- Function: ", BG_BLACK | FG_WHITE | FG_INTENSITY); */
/* 	      memzero(buffer, 32); */
/* 	      itoa(buffer, function, 10); */
/* 	      printf(buffer, BG_BLACK | FG_WHITE | FG_INTENSITY); */
/* 	      printf("\tId = 0x", */
/* 		     BG_BLACK | FG_WHITE | FG_INTENSITY); */
/* 	      memzero(buffer, 32); */
/* 	      itoa(buffer, (v & ~0xffff) >> 16, BASE_HEX); */
/* 	      printf(buffer,  BG_BLACK | FG_WHITE | FG_INTENSITY); */
/* 	      printf("\n", 0); */
/* 	    } */
/* 	} */
/*       device_found = false; */
/*     } */
/* #endif /\* !__PCI_LIST__ *\/ */
/* } */


/* int32_t		test_ide_dma(void) */
/* { */
/*   //uint16_t	bm_interface; */

/* #ifdef _TEST_IDE_DMA_ */
/* uint8_t	i; */
/*   uint32_t_t	value = 0; */
/*   char		buffer[32]; */

/*   /\* list PCI device and bus *\/ */


/*   /\* device vendor:id *\/ */
/*   printf("Device vendor & id :\t", BG_BLACK | PCI_FG | FG_INTENSITY); */
/*   value = (0x80000000 | (0 << 11) | (57 << 8)); */
/*   OUTL(value, 0x0cf8); */
/*   INL(&value, 0x0cfc); */
/*   if ((value & 0xffff) == 0x8086) */
/*     printf("Intel-", BG_BLACK | PCI_FG | FG_INTENSITY); */
/*   else */
/*     { */
/*       memzero(buffer, 32); */
/*       itoa(buffer, value & 0xffff, BASE_HEX); */
/*       printf(buffer,  BG_BLACK | PCI_FG | FG_INTENSITY); */
/*       printf("-", BG_BLACK | PCI_FG | FG_INTENSITY); */
/*     } */
/*   if (((value & ~0xffff) >> 16) == 0x7010) */
/*     printf("PIIX3\n", BG_BLACK | PCI_FG | FG_INTENSITY); */
/*   else */
/*     { */
/*       memzero(buffer, 32); */
/*       itoa(buffer, (value & ~0xffff) >> 16, BASE_HEX); */
/*       printf(buffer,  BG_BLACK | PCI_FG | FG_INTENSITY); */
/*       printf("\n", BG_BLACK | PCI_FG | FG_INTENSITY); */
/*     } */

/*   /\* Bus Master Interface Base Address *\/ */
/*   printf("Bus master interface :\t0x", BG_BLACK | PCI_FG | FG_INTENSITY); */
/*   value = (0x80000000 | (0 << 11) | (57 << 8) | 0x20); */
/*   OUTL(value, 0x0cf8); */
/*   INL(&value, 0x0cfc); */
/*   memzero(buffer, 32); */
/*   bm_interface = value & 0xfffe; */
/*   itoa(buffer, bm_interface, BASE_HEX); */
/*   printf(buffer,  BG_BLACK | PCI_FG | FG_INTENSITY); */
/*   if (value & 1) */
/*     printf (" (I/O mapped)\n", BG_BLACK | PCI_FG | FG_INTENSITY); */
/*   else */
/*     printf (" (memory mapped)\n", BG_BLACK | PCI_FG | FG_INTENSITY); */

/*   /\* PCI command register *\/ */
/*   printf("PCICMD register :\t\t0x", BG_BLACK | PCI_FG | FG_INTENSITY); */
/*   value = (0x80000000 | (0 << 11) | (57 << 8) | 0x4); */
/*   OUTL(value, 0x0cf8); */
/*   INW(&value, 0x0cfc); */
/*   value &= ~3; // deactivate I/O space and Memory */
/*   //value |= (1 << 2); // Bus Master Enable bit */
/*   OUTW(value, 0x0cfc); */
/*   INW(&value, 0x0cfc); */
/*   memzero(buffer, 32); */
/*   itoa(buffer, value & 0xffff, BASE_HEX); */
/*   printf(buffer,  BG_BLACK | PCI_FG | FG_INTENSITY); */
/*   printf ("\n", 0); */

/*   /\* IDE Timing register *\/ */
/* /\*   printf("IDETIM register :\t\t0x", BG_BLACK | PCI_FG | FG_INTENSITY); *\/ */
/* /\*   value = (0x80000000 | (1 << 11) | (1 << 8) | 0x40); *\/ */
/* /\*   OUTL(value, 0x0cf8); *\/ */
/* /\*   value = 0x8088; *\/ */
/* /\*   OUTW(value, 0x0cfc); *\/ */
/* /\*   INW(&value, 0x0cfc); *\/ */
/* /\*   value &= 0xffff; *\/ */
/* /\*   memzero(buffer, 32); *\/ */
/* /\*   itoa(buffer, value, BASE_HEX); *\/ */
/* /\*   printf(buffer,  BG_BLACK | PCI_FG | FG_INTENSITY); *\/ */
/* /\*   printf ("\n", 0); *\/ */

/*   /\* Class Code register (3 bytes) *\/ */
/* /\*   printf("CLASSC register :\t\t0x", BG_BLACK | PCI_FG | FG_INTENSITY); *\/ */
/* /\*   value = (0x80000000 | (1 << 11) | (1 << 8) | 0x8); *\/ */
/* /\*   OUTL(value, 0x0cf8); *\/ */
/* /\*   INL(&value, 0x0cfc); *\/ */
/* /\*   value >>= 8; *\/ */
/* /\*   value &= 0xffffff; *\/ */
/* /\*   value |= 0xf; *\/ */
/* /\*   value <<= 8; *\/ */
/* /\*   OUTL(value, 0x0cfc); *\/ */
/* /\*   INL(&value, 0x0cfc); *\/ */
/* /\*   memzero(buffer, 32); *\/ */
/* /\*   itoa(buffer, value, BASE_HEX); *\/ */
/* /\*   printf(buffer,  BG_BLACK | PCI_FG | FG_INTENSITY); *\/ */
/* /\*   printf ("\n", 0); *\/ */
/*   printf("CLASSC register :\t\t0x", BG_BLACK | PCI_FG | FG_INTENSITY); */
/*   value = (0x80000000 | (0 << 11) | (57 << 8) | 0x9); */
/*   OUTL(value, 0x0cf8); */
/*   value = 0; */
/*   INB(&value, 0x0cfc); */
/*   //value |= 0xf; */
/*   //OUTB(value, 0x0cfc); */
/*   //INB(&value, 0x0cfc); */
/*   memzero(buffer, 32); */
/*   itoa(buffer, value & 0xff, BASE_HEX); */
/*   printf(buffer,  BG_BLACK | PCI_FG | FG_INTENSITY); */
/*   printf ("\n", 0); */

/*   /\* Bus Master IDE Status register *\/ */
/*   printf("BMISTA register :\t\t0x", BG_BLACK | PCI_FG | FG_INTENSITY); */
/*   INB_VPORT(&value, bm_interface + 2); */
/*   value &= 0xff; */
/*   memzero(buffer, 32); */
/*   itoa(buffer, value, BASE_HEX); */
/*   printf(buffer,  BG_BLACK | PCI_FG | FG_INTENSITY); */
/*   printf ("\n", 0); */

/*   /\** */
/*    **	print I/O space */
/*    **\/ */
/*   i = 0; */
/*   while (i < 0x20) */
/*     { */
/*       printf("0x", BG_BLACK | PCI_FG | FG_INTENSITY); */
/*       memzero(buffer, 32); */
/*       itoa(buffer, i, BASE_HEX); */
/*       printf(buffer, BG_BLACK | PCI_FG | FG_INTENSITY); */
/*       printf("\t: ", BG_BLACK | PCI_FG | FG_INTENSITY); */

/*       value = (0x80000000 | (0 << 11) | (57 << 8) | i); */
/*       OUTL(value, 0x0cf8); */
/*       INL(&value, 0x0cfc); */

/*       memzero(buffer, 32); */
/*       itoa(buffer, value, BASE_HEX); */
/*       printf(buffer,  BG_BLACK | PCI_FG | FG_INTENSITY); */
/*       printf ("\n", 0); */

/*       i += 4; */
/*     } */

/* #endif /\* !_TEST_IDE_DMA_ *\/ */

/*   return 0; */
/* } */

/* int32_t		test_apic(void) */
/* { */
/* #ifdef _TEST_APIC_ */
/*   uint32_t	value = 0; */
/*   //char		buffer[32]; */
/*   //char		msr_apic_base_sz[128]; */

/*   /\* Local APIC Base Address Register (from MSR) *\/ */

/*   RDMSR(&value, MSR_IA32_APIC_BASE); */
/*   fprintf(stdout, "Local-APIC bar (from MSR):\t%r", value);   */
/*   if (value & (1 << 11)) */
/*     fprintf(stdout, " (enabled)\n"); */
/*   else */
/*     fprintf(stdout, " (disabled)\n"); */

/*   /\* Local APIC version *\/ */
/*   apic_read_register(&value, APIC_REG_VERSION); */
/*   fprintf(stdout, "Local-APIC version:\t\t%r\n", value);  */

/*   /\* Local APIC id *\/ */
/*   apic_read_register(&value, APIC_REG_ID); */
/*   fprintf(stdout, "Local-APIC id:\t\t%r\n", value); */

/*   /\* register: IO-APIC version *\/ */
/*   apic_io_version(&value); */
/*   fprintf(stdout, "IO-APIC version:\t\t%r\n", value); */

/* #endif /\* !_TEST_APIC_ *\/ */

/*   return 0; */
/* } */

/* /\* int32_t	test_apic(void) *\/ */
/* /\* { *\/ */
/* #ifdef _TEST_APIC_sss */
/*   uint32_t	value = 0; */
/*   char		buffer[32]; */
/*   char		msr_apic_base_sz[128]; */

/*   /\* Local APIC Base Address Register (from MSR) *\/ */
/*   printf("LAPIC BAR (from MSR) = 0x", BG_BLACK | FG_RED); */
/*   RDMSR(&value, MSR_IA32_APIC_BASE); */
/*   memzero(buffer, 32); */
/*   itoa(buffer, value & ~0xfff, BASE_HEX); */
/*   printf(buffer, BG_BLACK | FG_RED); */
/*   if (value & (1 << 11)) */
/*     printf(" (enabled)\n", BG_BLACK | FG_RED); */
/*   else */
/*     printf(" (disabled)\n", BG_BLACK | FG_RED); */

/*   /\* Local APIC version *\/ */
/*   apic_read_register(&value, APIC_REG_VERSION); */
/*   memzero(buffer, 32); */
/*   memzero(msr_apic_base_sz, 128); */
/*   itoa(buffer, value, BASE_HEX); */
/*   strcat(msr_apic_base_sz, "APIC version = 0x"); */
/*   strcat(msr_apic_base_sz, buffer); */
/*   fb_println(msr_apic_base_sz, BG_BLACK | FG_WHITE); */

/*   apic_read_register(&value, APIC_REG_ID); */
/*   memzero(buffer, 32); */
/*   memzero(msr_apic_base_sz, 128); */
/*   itoa(buffer, value, BASE_HEX); */
/*   strcat(msr_apic_base_sz, "APIC id = 0x"); */
/*   strcat(msr_apic_base_sz, buffer); */
/*   fb_println(msr_apic_base_sz, BG_BLACK | FG_WHITE); */
  
/*   apic_read_register(&value, APIC_REG_LVT_TIMER); */
/*   memzero(buffer, 32); */
/*   memzero(msr_apic_base_sz, 128); */
/*   itoa(buffer, value, BASE_HEX); */
/*   strcat(msr_apic_base_sz, "APIC timer = 0x"); */
/*   strcat(msr_apic_base_sz, buffer); */
/*   fb_println(msr_apic_base_sz, BG_BLACK | FG_WHITE); */
  
/*   /\* register: IO-APIC version *\/ */
/*   apic_io_version(&value); */
/*   memzero(buffer, 32); */
/*   itoa(buffer, value, BASE_HEX); */
/*   printf("IO-APIC version =\t0x", BG_BLACK | FG_WHITE); */
/*   printf(buffer, BG_BLACK | FG_WHITE | FG_INTENSITY); */
/*   printf("\n", 0); */

/* #endif /\* !_TEST_APIC_ *\/ */

/* /\*   return 0; *\/ */
/* /\* } *\/ */

/* int32_t	test_msr(void) */
/* { */
/* #ifdef _TEST_MSR_ */
/*   uint32_t	value = 0; */
/*   char		buffer[32]; */
/*   char		msr_apic_base_sz[128]; */
  
/*   RDMSR(&value, MSR_IA32_APIC_BASE); */
/*   memzero(buffer, 32); */
/*   memzero(msr_apic_base_sz, 32); */
/*   itoa(buffer, value, BASE_HEX); */
/*   strcat(msr_apic_base_sz, "MSR_APIC_BASE = 0x"); */
/*   strcat(msr_apic_base_sz, buffer); */
/*   fb_println(msr_apic_base_sz, BG_BLACK | FG_WHITE); */

/*   RDMSR(&value, MSR_IA32_THERM_STATUS); */
/*   memzero(buffer, 32); */
/*   memzero(msr_apic_base_sz, 32); */
/*   itoa(buffer, value, BASE_HEX); */
/*   strcat(msr_apic_base_sz, "MSR_IA32_THERM_STATUS = 0x"); */
/*   strcat(msr_apic_base_sz, buffer); */
/*   fb_println(msr_apic_base_sz, BG_BLACK | FG_WHITE); */

/*   RDMSR(&value, MSR_IA32_MISC_ENABLE); */
/*   memzero(buffer, 32); */
/*   memzero(msr_apic_base_sz, 32); */
/*   itoa(buffer, value, BASE_HEX); */
/*   strcat(msr_apic_base_sz, "MSR_IA32_MISC_ENABLE = 0x"); */
/*   strcat(msr_apic_base_sz, buffer); */
/*   fb_println(msr_apic_base_sz, BG_BLACK | FG_WHITE); */

/*   RDMSR(&value, MSR_IA32_PLATFORM_ID); */
/*   memzero(buffer, 32); */
/*   memzero(msr_apic_base_sz, 32); */
/*   itoa(buffer, value, BASE_HEX); */
/*   strcat(msr_apic_base_sz, "MSR_IA32_PLATFORM_ID = 0x"); */
/*   strcat(msr_apic_base_sz, buffer); */
/*   fb_println(msr_apic_base_sz, BG_BLACK | FG_WHITE); */

/*   RDMSR(&value, MSR_IA32_MPERF); */
/*   memzero(buffer, 32); */
/*   memzero(msr_apic_base_sz, 32); */
/*   itoa(buffer, value, BASE_HEX); */
/*   strcat(msr_apic_base_sz, "MSR_IA32_MPERF = 0x"); */
/*   strcat(msr_apic_base_sz, buffer); */
/*   fb_println(msr_apic_base_sz, BG_BLACK | FG_WHITE); */

/*   RDMSR(&value, MSR_IA32_APERF); */
/*   memzero(buffer, 32); */
/*   memzero(msr_apic_base_sz, 32); */
/*   itoa(buffer, value, BASE_HEX); */
/*   strcat(msr_apic_base_sz, "MSR_IA32_APERF = 0x"); */
/*   strcat(msr_apic_base_sz, buffer); */
/*   fb_println(msr_apic_base_sz, BG_BLACK | FG_WHITE); */

/*   RDMSR(&value, MSR_IA32_TIME_STAMP_COUNTER); */
/*   memzero(buffer, 32); */
/*   memzero(msr_apic_base_sz, 128); */
/*   itoa(buffer, value, BASE_HEX); */
/*   strcat(msr_apic_base_sz, "MSR_IA32_TIME_STAMP_COUNTER = 0x"); */
/*   strcat(msr_apic_base_sz, buffer); */
/*   fb_println(msr_apic_base_sz, BG_BLACK | FG_WHITE); */
/* #endif /\* !_TEST_MSR_ *\/ */

/*   return 0; */
/* } */

/* int32_t	test_interrupts(void) */
/* { */
/* #ifdef _TEST_INTERRUPTS_ */
/*   __asm__ ("int $2"); */
/*   __asm__ ("int $15"); */
/*   __asm__ ("int $21"); */
/*   __asm__ ("int $65"); */

/*   fb_println("Back from interrupts...", BG_BLACK | FG_WHITE); */
/* #endif /\* !_TEST_INTERRUPTS_ *\/ */

/*   return 0; */
/* } */
