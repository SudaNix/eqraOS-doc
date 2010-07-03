#include <string.h>
#include "gdt.h"
 
static struct gdt_desc _gdt[MAX_GDT_DESC];
static struct gdtr _gdtr;


static void gdt_install();


static void gdt_install() {
#ifdef _MSC_VER
	_asm lgdt [_gdtr];
#endif
}

extern void i386_gdt_set_desc(uint32_t index,uint64_t base,uint64_t limit,uint8_t access,uint8_t grand) {
	
	if ( index > MAX_GDT_DESC )
		return;
		
	// clear the desc.
	memset((void*)&_gdt[index],0,sizeof(struct gdt_desc));
	
	// set limit and base.
	_gdt[index].low_base = uint16_t(base & 0xffff);
	_gdt[index].mid_base = uint8_t((base >> 16) & 0xff);
	_gdt[index].high_base = uint8_t((base >> 24) & 0xff);
	_gdt[index].limit = uint16_t(limit & 0xffff);
	
	// set flags and grandularity bytes
	_gdt[index].flags = access;
	_gdt[index].grand = uint8_t((limit >> 16) & 0x0f);
	_gdt[index].grand = _gdt[index].grand | grand & 0xf0;
}

extern gdt_desc* i386_get_gdt_desc(uint32_t index) {
	if ( index >= MAX_GDT_DESC )
		return 0;
	else
		return &_gdt[index];
}

extern int i386_gdt_init() {
	
	// init _gdtr
	_gdtr.limit = sizeof(struct gdt_desc) * MAX_GDT_DESC - 1;
	_gdtr.base = (uint32_t)&_gdt[0];
	
	// set null desc.
	i386_gdt_set_desc(0,0,0,0,0);
	
	// set code desc.
	i386_gdt_set_desc(1,0,0xffffffff,
		I386_GDT_CODE_DESC|I386_GDT_DATA_DESC|I386_GDT_READWRITE|I386_GDT_MEMORY, 	// 10011010
		I386_GDT_LIMIT_HI|I386_GDT_32BIT|I386_GDT_4K 								// 11001111
 
	);
	
	// set data desc.
	i386_gdt_set_desc(2,0,0xffffffff,
		I386_GDT_DATA_DESC|I386_GDT_READWRITE|I386_GDT_MEMORY, 	// 10010010
		I386_GDT_LIMIT_HI|I386_GDT_32BIT|I386_GDT_4K			// 11001111
	);
	
	// install gdtr
	gdt_install();
	
	return 0;
}
