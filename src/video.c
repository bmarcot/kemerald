/* mandatory includes */
#include "types.h"
#include "kernel.h"
#include "video.h"

/* additional includes */
#include "asm.h"
#include "port.h"
#include "stdio.h"
#include "mmu.h"


int32_t		video_init(void)
{
  uint16_t	val;
  uint32_t	i;
  uint32_t	fb_size;

  /* Get the BGA version. */
  outw(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_ID);
  val = inw(VBE_DISPI_IOPORT_DATA);
  printf("BGA version: %x\n", val);

 /*  if (val != VBE_DISPI_ID4) */
/*     { */
/*       return ERR_UNKNOWN; */
/*     } */

  /* XXX: Mapper en 'identity' le framebuffer. */

  /* Set the framebuffer size. */
  fb_size = 1024 * 768 * 4;

 /*  printf("before reserve ok @=%x, opts=%x\n", VBE_DISPI_LFB_PHYSICAL_ADDRESS, */
/* 	 MMU_PAGE_OPT_READWRITE */
/* 	 | MMU_PAGE_OPT_WRITETROUGH */
/* 	 | MMU_PAGE_OPT_CACHE_DISABLE */
/* 	 | MMU_PAGE_OPT_IDENTITY); */
  
  /* Reserve the video ram pages (mapped in identity). */
  for (i = 0; i < fb_size / MMU_PAGE_SIZE; i++)
    {
      printf("Page reserve... %x\n", VBE_DISPI_LFB_PHYSICAL_ADDRESS + i * MMU_PAGE_SIZE);
      if (mmu_virt_page_reserve((vaddr_t)(VBE_DISPI_LFB_PHYSICAL_ADDRESS + i * MMU_PAGE_SIZE),
				MMU_PAGE_OPT_READWRITE
				| MMU_PAGE_OPT_WRITETROUGH
				| MMU_PAGE_OPT_CACHE_DISABLE
				| MMU_PAGE_OPT_IDENTITY) != ERR_NONE)
	{
	  printf("Cant reserve video RAM pages...\n");
	  INFINITE_LOOP();
	}
      //printf("first reserve ok\n");
      //INFINITE_LOOP();
    }

  //INFINITE_LOOP();

  /* Disable VBE. */
  outw(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_ENABLE);
  outw(VBE_DISPI_IOPORT_DATA, VBE_DISPI_DISABLED);

  /* Write Xres and Yres. */
  outw(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_XRES);
  outw(VBE_DISPI_IOPORT_DATA, 1024);
  outw(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_YRES);
  outw(VBE_DISPI_IOPORT_DATA, 768);

  /* Set the BPP. */
  outw(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_BPP);
  outw(VBE_DISPI_IOPORT_DATA, VBE_DISPI_BPP_32);

  /* Enable VBE. */
  /* outw(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_ENABLE); */
/*   outw(VBE_DISPI_IOPORT_DATA, VBE_DISPI_ENABLED); */

  outw(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_XRES);
  val = inw(VBE_DISPI_IOPORT_DATA);
  printf("Xres: %d\n", val);
  outw(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_YRES);
  val = inw(VBE_DISPI_IOPORT_DATA);
  printf("Yres: %d\n", val);
  outw(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_ENABLE);
  val = inw(VBE_DISPI_IOPORT_DATA);
  printf("Enable: %d\n", val);


/*   while (1) */
/*     ; */

  return ERR_NONE;
}
