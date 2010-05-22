#include "gdt.h"
#include "idt.h"
#include "cpu.h"

int i386_cpu_init() {
	
	i386_gdt_init();
	i386_idt_init(0x8);
	
	return 0;
}

void i386_cpu_close() {
}


char* i386_cpu_vendor() {
	static char ven[32] = {0};
	
#ifndef _MSC_VER
	_asm {
		xor eax,eax
		cpuid
		mov dword ptr [ven],ebx
		mov dword ptr [ven+4],edx
		mov dowrd ptr [ven+8],ecx
	}
#endif

	return ven;
}