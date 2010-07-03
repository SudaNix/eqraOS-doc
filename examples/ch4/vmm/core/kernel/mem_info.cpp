#include "mem_info.h"

#define KERNEL_BASE			0x100000
#define KERNEL_VIR_BASE		0xC0000000

struct mem_region {
	uint32_t base_low;
	uint32_t base_high;
	uint32_t length_low;
	uint32_t length_high;
	uint32_t type;
	uint32_t acpi;
};

char* mem_types[] = {
	{"available"},
	{"reserved"},
	{"acpi reclaim"},
	{"acpi nvs memory"}
};

uint32_t kernel_size = 0;
uint32_t mem_size = 0;
multiboot_info* boot_info;
mem_region* region_ptr = (mem_region*) 0x1000;

void mem_info_init(multiboot_info* info) {

	// Multiboot informtion.
	boot_info = info;

	// kernel size
	_asm mov word ptr [kernel_size],dx
	
	// Memory size in KB
	// mem_size = 1024 + boot_info->m_mem_low + boot_info->m_mem_high * 64; computed from bootloader .
	mem_size = boot_info->m_mem_size;
	
	// Init pmm, put memory map bit above the kernel.
	pmm_init(mem_size,KERNEL_BASE + kernel_size*512);

	// Unset all non-reserved regions.
	mem_info_init_region();

	// kernel region is reserved (set).
	pmm_deinit_region(KERNEL_BASE,kernel_size*512);
}


void mem_info_init_region() {

	for (int i=0;i<15;++i) {
		
		// if region is unefined make it reserved.
		if (region_ptr[i].type > 4) 
			region_ptr[i].type = 1;
			
		// if there is no more region break.
		if (i>0 && region_ptr[i].base_low == 0)
			break;
			
		// init region if available
		if (region_ptr[i].type == 1)
			pmm_init_region(region_ptr[i].base_low,region_ptr[i].length_low);
	}
}

void mem_info_dump_region() {
	kset_color(0x19);
	kputs("Physical Memory Map\n");
	kputs("region      base        size        type\n");
	kputs("---------------------------------------------------\n\n");
	
	for (int i=0;i<15;++i) {
	
		// if there is no more region break.
		if (i>0 && region_ptr[i].base_low == 0)
			break;
			
		// dump entry.
		kprintf("%d            0x%x%x          0x%x%x        %s\n",
			i,
			region_ptr[i].base_high,region_ptr[i].base_low,
			region_ptr[i].length_high,region_ptr[i].length_low,
			mem_types[region_ptr[i].type-1]
			);
	}
}

void mem_info_dump_size() {
	kprintf("Physical memory size: %d KB\n",mem_size);
	//kprintf("memory low: %d    ;  memory high: %d \n\n",boot_info->m_mem_low,boot_info->m_mem_high);
}

void mem_info_blocks_stat() {
	kset_color(GRAY_ON_BLUE);
	
	kprintf("Memory statistics: all blocks: %d  ; used blocks: %d  ; free blocks: %d\n",
		pmm_get_block_count(),
		pmm_get_used_block_count(),
		pmm_get_free_block_count()
		);
}