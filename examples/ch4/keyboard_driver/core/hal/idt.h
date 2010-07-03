#ifndef IDT_H
#define IDT_H

#ifndef i386
#error "[idt.h] platfrom not implemented."
#endif

#include <stdint.h>

#define I386_MAX_INT		256			// 265 ir.
#define I386_IDT_16BIT		0x06
#define I386_IDT_32BIT		0x0e
#define I386_IDT_RING1		0x40
#define I386_IDT_RING2		0x20
#define I386_IDT_RING3		0x60
#define I386_IDT_PRESENT	0x80

typedef void (_cdecl *I386_IRQ_HANDLER)(void);

#ifdef _MSC_VER
#pragma pack (push,1)
#endif

struct idt_desc {
	uint16_t base_low;
	uint16_t selector;
	uint8_t  reserved;
	uint8_t	 flags;
	uint16_t base_high;
};

struct idtr {
	uint16_t limit;
	uint32_t base;
};

#ifdef _MSC_VER
#pragma pack (pop)
#endif
	
/*	Interface	*/

extern int i386_idt_init(uint16_t code_selector);
extern int i386_idt_install_ir(uint32_t index,uint16_t flags,uint16_t sel,I386_IRQ_HANDLER);
extern idt_desc* i386_get_idt_ir(uint32_t index);

	
#endif // IDT_H