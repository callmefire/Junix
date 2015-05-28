/* kernel.c - the C part of the kernel */

#include <multiboot.h>
#include <config.h>
#include <stdio.h>
#include <stdlib.h>

#include <asm/cpu.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

int get_mbi(unsigned long magic, unsigned long addr)
{
    multiboot_info_t *mbi;

    /* Check Multiboot Magic  */
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
	    printk("Invalid magic number: 0x%x\n", (unsigned) magic);
	    return 1;
    }
    
    /* Set MBI to the address of the Multiboot information structure.  */
    mbi = (multiboot_info_t *) addr;

	printk("MBI Infomation\n");

    /* Are mem_* valid?  */
    if (CHECK_FLAG(mbi->flags, 0))
		printk("    Lower memory: %uKB,  Upper memory: %uKB\n",
	          (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

    /* Is boot_device valid?  */
    if (CHECK_FLAG(mbi->flags, 1))
		printk("    Boot Device ID: 0x%x\n", (unsigned) (mbi->boot_device >> 24) & 0xff);

    /* Is the command line passed?  */
    if (CHECK_FLAG(mbi->flags, 2))
		printk("    Command line: %s\n", (char *) mbi->cmdline);

    /* Are mods_* valid?  */
    if (CHECK_FLAG(mbi->flags, 3)) {
		module_t *mod;
		int i;

		printk("    Module Count: %d\n",(int) mbi->mods_count);
		for (i = 0, mod = (module_t *) mbi->mods_addr; i < mbi->mods_count; i++, mod++)
	    	printk("     %s\n",(char *) mod->string);
		printk("\n");
    }

    /* Bits 4 and 5 are mutually exclusive!  */
    if (CHECK_FLAG(mbi->flags, 4) && CHECK_FLAG(mbi->flags, 5)) {
		printk("Both bits 4 and 5 are set.\n");
		return 1;
    }

    /* Are mmap_* valid?  */
    if (CHECK_FLAG(mbi->flags, 6)) {
		memory_map_t *mmap;

		printk("    BIOS-provided physical RAM map:\n");

		for (mmap = (memory_map_t *) mbi->mmap_addr;
	        (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
	    	mmap = (memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size)))
	    	
			printk("        BIOS-e820:   %x%x - %x%x  (%s)\n",
		   		(unsigned) mmap->base_addr_high,
		   		(unsigned) mmap->base_addr_low,
		   		(unsigned) mmap->base_addr_high + mmap->length_high,
		   		(unsigned) mmap->base_addr_low + mmap->length_low,
				(unsigned) mmap->type == 1? "RAM":"Reserved");
    }

    return 0;
}

void main(unsigned long magic, unsigned long addr)
{
	video_init();

    printk("Junix(0.1)\n");

    if (get_mbi(magic,addr))
		return;
    
	get_cpu_type();

	while (1) {
		;
	}

}


