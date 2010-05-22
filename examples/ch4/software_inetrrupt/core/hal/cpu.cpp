#include "gdt.h"
#include "idt.h"
#include "cpu.h"

extern int i386_cpu_init() {
	
	i386_gdt_init();
	i386_idt_init(0x8);
	
	return 0;
}

extern void i386_cpu_close() {
}