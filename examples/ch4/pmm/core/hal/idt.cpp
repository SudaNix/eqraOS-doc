#include <hal.h>
#include <string.h>

#include "idt.h"

#ifdef _DEBUG
#include "..\kernel\kdisplay.h"
#endif

static struct idt_desc _idt[I386_MAX_INT];
static struct idtr _idtr;

static void idt_install();
static void i386_default_handler();

static void idt_install() {
#ifdef _MSC_VER
	_asm lidt [_idtr];
#endif
}

static void i386_default_handler() {
#ifdef _DEBUG
	kclear(0x18);
	kgoto_xy(0,0);
	kset_color(0x1e);
	kputs("**** [I386 HAL] i386_default_handler: Unhandled Exception");
#endif

	for (;;);	// stucking here.
}


extern int i386_idt_init(uint16_t code_selector) {
	
	// init _idtr
	_idtr.limit = sizeof(struct idt_desc) * I386_MAX_INT - 1;
	_idtr.base = (uint32_t)&_idt[0];
	
	// clear all idt
	memset((void*)&_idt[0],0,sizeof(struct idt_desc)*I386_MAX_INT-1);
	
	// register default handlers.
	for (int i=0;i<I386_MAX_INT;++i) {
		i386_idt_install_ir(i,I386_IDT_32BIT|I386_IDT_PRESENT /*10001110*/,code_selector,(I386_IRQ_HANDLER)i386_default_handler);
	}
	
	// install idt
	idt_install();
	
	return 0;
}

extern int i386_idt_install_ir(uint32_t index,uint16_t flags,uint16_t sel,I386_IRQ_HANDLER irq) {
	if (index > I386_MAX_INT)
		return 0;
	
	if (!irq)
		return 0;
		
	uint64_t irq_base = (uint64_t)&(*irq);
	
	_idt[index].base_low = uint16_t(irq_base & 0xffff);
	_idt[index].base_high = uint16_t((irq_base >> 16) & 0xffff);
	_idt[index].reserved = 0;
	_idt[index].flags = uint8_t(flags);
	_idt[index].selector = sel;
	
	return 0;
}

extern idt_desc* i386_get_idt_ir(uint32_t index) {
	if (index > I386_MAX_INT)
		return 0;
	
	return &_idt[index];
}


