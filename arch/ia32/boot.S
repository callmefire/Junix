/* boot.S - bootstrap the kernel */

#define ASM	1
#include <config.h>
#include <multiboot.h>
	
#define BOOT_CS	 0x8
#define BOOT_DS	 0x10

.text
.globl	start, _start

start:
_start:
	/* For Grub, we can use it's GDT to do a long jmp.
	 * But to be compliant with other bootloader, setup own GDT.
	 */
	cld
	lgdt boot_gdt_descr - PAGE_OFFSET
	movl $(BOOT_DS),%edx
        movl %edx,%ds
        movl %edx,%es
        movl %edx,%fs
        movl %edx,%gs 
	ljmp	$(BOOT_CS), $multiboot_entry - PAGE_OFFSET
	
	/* Align 32 bits boundary.  */
	.align	4
	
	/* Multiboot header.  */
multiboot_header:
	/* magic */
	.long	MULTIBOOT_HEADER_MAGIC
	/* flags */
	.long	MULTIBOOT_HEADER_FLAGS
	/* checksum */
	.long	-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
	/* header_addr */
	.long	multiboot_header
	/* load_addr */
	.long	_start
	/* load_end_addr */
	.long	_edata
	/* bss_end_addr */
	.long	_end
	/* entry_addr , Grub doesn't care this */
	.long	multiboot_entry

multiboot_entry:

	/* Initialize the stack pointer.  */
	movl	$(init_stack + INIT_STACK_SIZE - PAGE_OFFSET), %esp
	
	/* Setup init page table */

	pushl %eax
	pushl %ecx
	pushl %edx
	pushl %edi

	movl $(pg0 - PAGE_OFFSET), %edi
	movl $(swapper_pg_dir - PAGE_OFFSET), %edx
	movl $0x007, %eax

	/* 1st pde: 0-4M */
	leal 0x007(%edi),%ecx
	movl %ecx, (%edx)			/* map 0 - 1M region with the same phy page(pg0) */
	movl %ecx, KERNEL_PDE_OFFSET(%edx)
	movl $1024,%ecx
1:
	stosl
	addl $0x1000, %eax
	loop 1b

	/* 2nd pde: 4-8M */
	leal 0x007(%edi),%ecx	
	addl $0x4, %edx
	movl %ecx, KERNEL_PDE_OFFSET(%edx)
	movl $1024,%ecx
2:	
	stosl
	addl $0x1000, %eax
	loop 2b

	popl %edi
	popl %edx
	popl %ecx
	popl %eax

	movl $(swapper_pg_dir - PAGE_OFFSET),%edx
	movl %edx,%cr3
	movl %cr0,%edx
	orl $0x80000000,%edx
	movl %edx,%cr0

	ljmp $BOOT_CS,$3f

3:
	/* Reinitialize the stack pointer.  */
	movl	$(init_stack + INIT_STACK_SIZE), %esp

	/* Reset EFLAGS.  */
	pushl	$0
	popf

	/* Push the pointer to the Multiboot information structure.  */
	pushl	%ebx
	/* Push the magic value.  */
	pushl	%eax

	/* Now enter the C main function...  */
	call	EXT_C(main)

	pushl	$halt_message
	call	EXT_C(printf)
	
loop:	hlt
	jmp	loop

halt_message:
	.asciz	"Halted."

.data
	.align 4
boot_gdt_descr:
	/* 8(Bytes) * 3(entry) = 24 */
        .word 0x18
        .long boot_gdt - PAGE_OFFSET

	/* Align 8 is enough. Here wants higher performance on SMP */
	.align L1_CACHE_BYTES
boot_gdt:
 	.quad 0x0000000000000000	/* NULL Segment */
        .quad 0x00cf9a000000ffff        /* kernel 4GB code at 0x00000000 */
        .quad 0x00cf92000000ffff        /* kernel 4GB data at 0x00000000 */	

	.align 0x1000
	/* Our stack area.  */
	.comm	init_stack, INIT_STACK_SIZE

.bss
	.align 0x1000
swapper_pg_dir:
	.fill	1024,4,0

	
