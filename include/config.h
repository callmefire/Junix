#ifndef CONFIG_H
#define CONFIG_H

#define IMAGE_PHY_LOAD   0x100000
#define PAGE_OFFSET	 0xC0000000

/* Famous 768 */
#define KERNEL_PDE_OFFSET       (PAGE_OFFSET >> 20)
#define KERNEL_PDE_INDEX	(PAGE_OFFSET >> 22)

/* Initial page table map size: 0 - 8M */
#define INIT_PDE_ENTRIES 0x2

/* Initial stack size: 8KB */
#define INIT_STACK_SIZE	 0x2000

#define L1_CACHE_BYTES	 128
#endif
