#ifndef CONFIG_H
#define CONFIG_H

/* physical load address */
#define IMAGE_PHY_LOAD   0x100000

/* Kernel space start address */
#define PAGE_OFFSET	 0xC0000000

/* Famous 768, in page directory table */
#define KERNEL_PDE_INDEX	(PAGE_OFFSET >> 22)
/* KERNEL_PDE_INDEX x 4 in page directory table */
#define KERNEL_PDE_OFFSET       (PAGE_OFFSET >> 20)

/* Initial page table map size: 0 - 8M */
#define INIT_PDE_ENTRIES 0x2

/* Initial stack size: 8KB */
#define INIT_STACK_SIZE	 0x2000

/* Arch infos */
#define L1_CACHE_BYTES	 64
#define L2_CACHE_BYTES	 64

/* Video mode during bootup 
 * 0 - TEXT Mode
 * 1 - Graphic mode 320x200x256
 * 2 - Graphic mode 640x480x16
 */
#define CONFIG_VIDEO_MODE	0	 

#endif
