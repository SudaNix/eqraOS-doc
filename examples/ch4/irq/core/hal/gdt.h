#ifndef GDT_H
#define GDT_H

#ifndef i386
#error "[gdt.h] platfrom not implemented."
#endif

#include <stdint.h>

#define MAX_GDT_DESC				3

#define I386_GDT_ACCESS			0x0001
#define I386_GDT_READWRITE		0x0002
#define I386_GDT_EXPANSION		0x0004
#define I386_GDT_CODE_DESC		0x0008
#define I386_GDT_DATA_DESC		0x0010
#define I386_GDT_DPLBIT			0x0060
#define I386_GDT_MEMORY			0x0080

/*	gdt descriptor grandularity bit flags */

#define I386_GDT_LIMIT_HI		0x0f
#define I386_GDT_OS				0x10
#define I386_GDT_32BIT			0x40
#define I386_GDT_4K				0x80


#ifdef _MSC_VER
#pragma pack (push,1)
#endif

struct gdt_desc {
	uint16_t	limit;
	uint16_t	low_base;
	uint8_t		mid_base;
	uint8_t		flags;
	uint8_t		grand;
	uint8_t		high_base;
};


struct gdtr {
	uint16_t	limit;
	uint32_t	base;
};

#ifdef _MSC_VER
#pragma pack (pop)
#endif


/* Interface */

extern void i386_gdt_set_desc(uint32_t index,uint64_t base,uint64_t limit,uint8_t access,uint8_t grand);
extern gdt_desc* i386_get_gdt_desc(uint32_t index);
extern int i386_gdt_init();


#endif // GDT_H