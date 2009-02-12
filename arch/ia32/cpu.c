#include <stdio.h>
#include <stdlib.h>

typedef struct cpuid_info {
	char vender[16];
	char family;
	char model;
	char stepping;
	unsigned char itlb_idx;
	unsigned char dtlb_idx;
	unsigned char il1_idx;
	unsigned char dl1_idx;
	unsigned char l2_idx;
	unsigned char l3_idx;
	unsigned char tc_idx;
	unsigned char pref_idx;
} cpuid_info_t;

typedef struct cpuid_cache {
	unsigned char code;
#define CPUID_CACHE_TYPE_ITLB 		0x01
#define CPUID_CACHE_TYPE_DTLB 		0x02
#define CPUID_CACHE_TYPE_ITLB4K 	0x03
#define CPUID_CACHE_TYPE_DTLB4K 	0x04
#define CPUID_CACHE_TYPE_ITLB4M 	0x05
#define CPUID_CACHE_TYPE_DTLB4M 	0x06
#define CPUID_CACHE_TYPE_IL1  		0x07
#define CPUID_CACHE_TYPE_DL1  		0x08
#define CPUID_CACHE_TYPE_L2  		0x09
#define CPUID_CACHE_TYPE_L3  		0x0A
#define CPUID_CACHE_TYPE_TC		  	0x0B
#define CPUID_CACHE_TYPE_PREFETCH 	0x0C
#define CPUID_CACHE_TYPE_NULL		0xFF
	unsigned char  type;		/* TLB? L1? L2? L3? I? D? Prefetch? */
	unsigned short size;		/* cache/TLB/Trace cache:KB Prefetch:B  */
	unsigned short ways;		/* Associative */
	unsigned short entries;  	/* For TLB: entry number  For cache: cacheline size */
} cpuid_cache_t;

static cpuid_cache_t cache_table[] = 
{
/*  code	type						size	ways	entries		*/
{	0x00,	CPUID_CACHE_TYPE_NULL,		0,		0,		0		  	},
{	0x01,	CPUID_CACHE_TYPE_ITLB4K,	0,		4,		32		  	},
{	0x02,	CPUID_CACHE_TYPE_ITLB4M,	0,		4,		2			},
{	0x03,	CPUID_CACHE_TYPE_DTLB4K,	0,		4,		64			},
{	0x04,	CPUID_CACHE_TYPE_DTLB4M,	0,		4,		8		    },
{	0x06,	CPUID_CACHE_TYPE_IL1,		8,		4,		32			},
{	0x08,	CPUID_CACHE_TYPE_IL1,		16,		4,		32			},
{	0x0A,	CPUID_CACHE_TYPE_DL1,		8,		2,		32			},
{	0x0C,	CPUID_CACHE_TYPE_DL1,		16,		4,		32			},
{	0x22,	CPUID_CACHE_TYPE_L3,		512,	4,		64			},
{	0x23,	CPUID_CACHE_TYPE_L3,		1024,	8,		64			},
{	0x25,	CPUID_CACHE_TYPE_L3,		2048,	8,		64			},
{	0x29,	CPUID_CACHE_TYPE_L3,		4096,	8,		64			},
{	0x2C,	CPUID_CACHE_TYPE_DL1,		32,		8,		64			},
{	0x30,	CPUID_CACHE_TYPE_IL1,		32,		8,		64			},
{	0x40,	CPUID_CACHE_TYPE_NULL,		0,		0,		0			},
{	0x41,	CPUID_CACHE_TYPE_L2,		128,	4,		32			},
{	0x42,	CPUID_CACHE_TYPE_L2,		256,	4,		32			},
{	0x43,	CPUID_CACHE_TYPE_L2,		512,	4,		32			},
{	0x44,	CPUID_CACHE_TYPE_L2,		1024,	4,		32			},
{	0x45,	CPUID_CACHE_TYPE_L2,		2048,	4,		32			},
{	0x46,	CPUID_CACHE_TYPE_L3,		4096,	4,		64			},
{	0x47,	CPUID_CACHE_TYPE_L3,		8192,	8,		64			},
{	0x50,	CPUID_CACHE_TYPE_ITLB,		0,		0,		64			},
{	0x51,	CPUID_CACHE_TYPE_ITLB,		0,		0,		128			},
{	0x52,	CPUID_CACHE_TYPE_ITLB,		0,		0,		256			},
{	0x5B,	CPUID_CACHE_TYPE_DTLB,		0,		0,		64			},
{	0x5C,	CPUID_CACHE_TYPE_DTLB,		0,		0,		128			},
{	0x5D,	CPUID_CACHE_TYPE_DTLB,		0,		0,		256			},
{	0x60,	CPUID_CACHE_TYPE_DL1,		16,		8,		64			},
{	0x66,	CPUID_CACHE_TYPE_DL1,		8,		4,		64			},
{	0x67,	CPUID_CACHE_TYPE_DL1,		16,		4,		64			},
{	0x68,	CPUID_CACHE_TYPE_DL1,		32,		4,		64			},
{	0x70,	CPUID_CACHE_TYPE_TC,		12,		8,		0			},
{	0x71,	CPUID_CACHE_TYPE_TC,		16,		8,		0			},
{	0x72,	CPUID_CACHE_TYPE_TC,		32,		8,		0			},
{	0x78,	CPUID_CACHE_TYPE_L2,		1024,	4,		64			},
{	0x79,	CPUID_CACHE_TYPE_L2,		128,	8,		64			},
{	0x7A,	CPUID_CACHE_TYPE_L2,		256,	8,		64			},
{	0x7B,	CPUID_CACHE_TYPE_L2,		512,	8,		64			},
{	0x7C,	CPUID_CACHE_TYPE_L2,		1024,	8,		64			},
{	0x7D,	CPUID_CACHE_TYPE_L2,		2048,	8,		64			},
{	0x7F,	CPUID_CACHE_TYPE_L2,		512,	2,		64			},
{	0x82,	CPUID_CACHE_TYPE_L2,		256,	8,		32			},
{	0x83,	CPUID_CACHE_TYPE_L2,		512,	8,		32			},
{	0x84,	CPUID_CACHE_TYPE_L2,		1024,	8,		32			},
{	0x85,	CPUID_CACHE_TYPE_L2,		2048,	8,		32			},
{	0x86,	CPUID_CACHE_TYPE_L2,		512,	4,		64			},
{	0x87,	CPUID_CACHE_TYPE_L2,		1024,	8,		64			},
{	0xB0,	CPUID_CACHE_TYPE_ITLB4K,	0,		4,		128			},
{	0xB0,	CPUID_CACHE_TYPE_DTLB4K,	0,		4,		128			},
{	0xF0,	CPUID_CACHE_TYPE_PREFETCH,	0,		0,		64			},
{	0xF1,	CPUID_CACHE_TYPE_PREFETCH,	0,		0,		128			}
};

unsigned int get_eflags(void)
{
	unsigned int ret = 0;

	__asm__ __volatile__ (
		"pushfl\n\t"
		"popl %0"
		: "=r"(ret)
		:
	);

	return ret;	
}

void set_eflags(unsigned int value)
{
	__asm__ __volatile__ (
		"pushl %0\n\t"
		"popfl"
		: 
		: "r"(value)
	);
}

/* Touch bit21 of EFLAGS */
static int support_cpuid(void)
{
	unsigned int eflags = 0;

	eflags = get_eflags();
	eflags |= 0x200000;
	set_eflags(eflags);
	
	eflags = get_eflags();
	if ( (eflags & 0x200000) == 0)
		return 0;

	eflags &= ~0x200000;
	set_eflags(eflags);

	eflags = get_eflags();
	if (eflags & 0x200000)
		return 0;

	return 1;
}

/* EAX = 0 */
void get_vender(char *vender)
{
	__asm__ __volatile__ (
		"pushl %%eax\n\t"
		"pushl %%ebx\n\t"
		"pushl %%ecx\n\t"
		"pushl %%edx\n\t"
		"movl $0x0, %%eax\n\t"
		"cpuid\n\t"
		"movl %%ebx,(%0)\n\t"
		"movl %%edx,4(%0)\n\t"
		"movl %%ecx,8(%0)\n\t"
		"popl %%edx\n\t" 
		"popl %%ecx\n\t" 
		"popl %%ebx\n\t" 
		"popl %%eax\n\t" 
		:
		:"D"(vender)
	);
}

unsigned int get_signature(void)
{
	unsigned int sig = 0;

	__asm__ __volatile__ (
		"pushl %%ebx\n\t"
		"pushl %%ecx\n\t"
		"pushl %%edx\n\t"
		"movl $0x1, %%eax\n\t"
		"cpuid\n\t"
		"popl %%edx\n\t"
		"popl %%ecx\n\t"
		"popl %%ebx\n\t"
		:"=a"(sig)
		:
	);

	return sig;
}

static unsigned char find_cache_type(unsigned char code)
{
	int i,size;

	size = sizeof(cache_table)/sizeof(cpuid_cache_t);
	for (i=0; i<size; i++) {
		if (cache_table[i].code == code)
			return i;
	}

	return 0;
}

#define get_type(x) cache_table[x].type
#define get_size(x) cache_table[x].size
#define get_ways(x) cache_table[x].ways
#define get_entries(x) cache_table[x].entries

void record_cache_type(unsigned int reg, cpuid_info_t *cpu_info) {
	unsigned char *c = NULL;
	unsigned char idx = 0;
	int i;

	if (!(reg & 0x80000000) ) {
		c = (unsigned char *)&reg;

		for (i=0; i<sizeof(reg); i++,c++) {

			idx = find_cache_type(c[0]);

			switch (get_type(idx)) {
				case CPUID_CACHE_TYPE_ITLB:
				case CPUID_CACHE_TYPE_ITLB4K:
				case CPUID_CACHE_TYPE_ITLB4M:
					cpu_info->itlb_idx = idx;
					break;
				case CPUID_CACHE_TYPE_DTLB:
				case CPUID_CACHE_TYPE_DTLB4K:
				case CPUID_CACHE_TYPE_DTLB4M:
					cpu_info->dtlb_idx = idx;
					break;
				case CPUID_CACHE_TYPE_IL1:
					cpu_info->il1_idx = idx;
					break;
				case CPUID_CACHE_TYPE_DL1:
					cpu_info->dl1_idx = idx;
					break;
				case CPUID_CACHE_TYPE_L2:
					cpu_info->l2_idx = idx;
					break;
				case CPUID_CACHE_TYPE_L3:
					cpu_info->l3_idx = idx;
					break;
				case CPUID_CACHE_TYPE_TC:
					cpu_info->tc_idx = idx;
					break;
				case CPUID_CACHE_TYPE_PREFETCH:
					cpu_info->pref_idx = idx;
					break;
				default:
					;	
			}
		}
	}
}

void show_cache_type(unsigned char idx)
{
	switch (get_type(idx)) {

		case CPUID_CACHE_TYPE_ITLB:
		
			printf("ITLB for 4K or 4M Page, ");
			if (get_ways(idx))
				printf("%d-ways associative, ",get_ways(idx));
			
			if (get_entries(idx))
				printf("%d entries",get_entries(idx));
			
			printf("\n");
			break;
		case CPUID_CACHE_TYPE_ITLB4K:
			
			printf("ITLB for 4K Page, ");
			if (get_ways(idx))
				printf("%d-ways associative ",get_ways(idx));
			
			if (get_entries(idx))
				printf("%d entries",get_entries(idx));
			
			printf("\n");
			break;
		case CPUID_CACHE_TYPE_ITLB4M:
			printf("ITLB for 4M Page, ");
			if (get_ways(idx))
				printf("%d-ways associative ",get_ways(idx));
			
			if (get_entries(idx))
				printf("%d entries",get_entries(idx));
			
			printf("\n");
			break;
		case CPUID_CACHE_TYPE_DTLB:
			
			printf("DTLB for 4K or 4M Page, ");
			if (get_ways(idx))
				printf("%d-ways associative ",get_ways(idx));
			
			if (get_entries(idx))
				printf("%d entries",get_entries(idx));
			
			printf("\n");
			break;
		case CPUID_CACHE_TYPE_DTLB4K:
			
			printf("DTLB for 4K Page, ");
			if (get_ways(idx))
				printf("%d-ways associative ",get_ways(idx));
			
			if (get_entries(idx))
				printf("%d entries",get_entries(idx));
			
			printf("\n");
			break;
		case CPUID_CACHE_TYPE_DTLB4M:
			
			printf("DTLB for 4M Page, ");
			if (get_ways(idx))
				printf("%d-ways associative ",get_ways(idx));
			
			if (get_entries(idx))
				printf("%d entries",get_entries(idx));
			
			printf("\n");
			break;
		case CPUID_CACHE_TYPE_IL1:
			
			printf("L1 ICache ");
			if (get_size(idx))
				printf("size %d KB ",get_size(idx));
			
			if (get_ways(idx))
				printf("%d-ways associative ",get_ways(idx));
			
			if (get_entries(idx))
				printf("cacheline %d",get_entries(idx));
			
			printf("\n");
			break;
		case CPUID_CACHE_TYPE_DL1:
			
			printf("L1 DCache ");
			if (get_size(idx))
				printf("%d KB ",get_size(idx));
			
			if (get_ways(idx))
				printf("%d-ways associative ",get_ways(idx));
			
			if (get_entries(idx))
				printf("cacheline size %d",get_entries(idx));
			
			printf("\n");
			break;
		case CPUID_CACHE_TYPE_L2:
			printf("L2 Cache ");
			if (get_size(idx))
				printf(" %d KB ",get_size(idx));
			
			if (get_ways(idx))
				printf("%d-ways associative ",get_ways(idx));
			
			if (get_entries(idx))
				printf("cacheline size %d",get_entries(idx));
			
			printf("\n");
			break;
		case CPUID_CACHE_TYPE_L3:
			
			printf("L3 Cache ");
			if (get_size(idx))
				printf("%d KB ",get_size(idx));
			
			if (get_ways(idx))
				printf("%d-ways associative ",get_ways(idx));
			
			if (get_entries(idx))
				printf("cacheline size %d",get_entries(idx));
			
			printf("\n");
			break;
		case CPUID_CACHE_TYPE_TC:
			
			printf("Trace Cache, ");
			if (get_size(idx))
				printf("%d Kuops ",get_size(idx));
			
			if (get_ways(idx))
				printf("%d-ways associative",get_ways(idx));
			
			printf("\n");
			break;
		case CPUID_CACHE_TYPE_PREFETCH:
			
			if (get_entries(idx))
				printf("Prefetch size %d\n",get_entries(idx));
			
			break;

		default:
			;	
	}
}

void get_cpu_cache(cpuid_info_t *cpu_info)
{
	unsigned int eax = 0;
    unsigned int ebx = 0;
	unsigned int ecx = 0;
	unsigned int edx = 0;
	int times;

	do {	

		__asm__ __volatile__(
			"pushl %%eax\n\t" 
			"pushl %%ebx\n\t" 
			"pushl %%ecx\n\t" 
			"pushl %%edx\n\t" 
			"movl $0x2, %%eax\n\t"
			"cpuid\n\t"
			"movl %%eax,%0\n\t"
			"movl %%ebx,%1\n\t"
			"movl %%ecx,%2\n\t"
			"movl %%edx,%3\n\t"
			"popl %%edx\n\t" 
			"popl %%ecx\n\t" 
			"popl %%ebx\n\t" 
			"popl %%eax\n\t" 
			:"=m"(eax),"=m"(ebx),"=m"(ecx),"=m"(edx)
			:   
		);  

		times = eax & 0xFF;
		eax &= ~0xFF;

		record_cache_type(eax,cpu_info);	
		record_cache_type(ebx,cpu_info);	
		record_cache_type(ecx,cpu_info);	
		record_cache_type(edx,cpu_info);	

	} while (times > 1);

	if (cpu_info->il1_idx)
		show_cache_type(cpu_info->il1_idx);

	if (cpu_info->tc_idx)
		show_cache_type(cpu_info->tc_idx);

	if (cpu_info->dl1_idx)
		show_cache_type(cpu_info->dl1_idx);
	
	if (cpu_info->l2_idx)
		show_cache_type(cpu_info->l2_idx);
	
	if (cpu_info->l3_idx)
		show_cache_type(cpu_info->l3_idx);
	
	if (cpu_info->itlb_idx)
		show_cache_type(cpu_info->itlb_idx);
	
	if (cpu_info->dtlb_idx)
		show_cache_type(cpu_info->dtlb_idx);
	
	if (cpu_info->pref_idx)
		show_cache_type(cpu_info->pref_idx);
}

#define EXT_FAMILY_MASK 			0x0FF00000
#define EXT_MODEL_MASK  			0x000F0000
#define PROCESSOR_TYPE_MASK 		0x00003000
#define PROCESSOR_FAMILY_MASK		0x00000F00
#define PROCESSOR_MODEL_MASK  		0x000000F0
#define PROCESSOR_STEPPING_MASK  	0x0000000F

#define PROCESSOR_TYPE_OEM  		0x0
#define PROCESSOR_TYPE_OVERDIRVE  	0x00001000
#define PROCESSOR_TYPE_DUAL 		0x00002000
#define PROCESSOR_TYPE_RESERVED  	0x00003000

void get_cpu_info(cpuid_info_t *cpu_info)
{
	unsigned int signature = 0;
	
	get_vender(cpu_info->vender);
	printf("%s  ",cpu_info->vender);

	signature = get_signature();	

	if ( (signature & PROCESSOR_TYPE_MASK) == PROCESSOR_TYPE_OEM)
		printf("OEM Processor  ");
	else if ( (signature & PROCESSOR_TYPE_MASK) == PROCESSOR_TYPE_OVERDIRVE)
		printf("Overdirve Processor  ");
	else if ( (signature & PROCESSOR_TYPE_MASK) == PROCESSOR_TYPE_DUAL)
		printf("Dual Processor  ");
	else 	
		printf("Reserved Type  ");

	cpu_info->family = (signature & PROCESSOR_FAMILY_MASK) >> 8;
	cpu_info->model  = (signature & PROCESSOR_MODEL_MASK) >> 4;
	cpu_info->stepping = (signature & PROCESSOR_STEPPING_MASK);
	printf("Family %d   Model %d  Stepping %d\n"
			, cpu_info->family , cpu_info->model , cpu_info->stepping);

	get_cpu_cache(cpu_info);

}

void get_cpu_ext_info(cpuid_info_t *cpu_info)
{

}

void get_cpu_type(void)
{
	cpuid_info_t cpu_info;

	if (!support_cpuid())
		return;

	memset(&cpu_info,0,sizeof(cpu_info));
	
	printf("CPU Supports CPUID:\n");

	get_cpu_info(&cpu_info);
	get_cpu_ext_info(&cpu_info);
	
}

