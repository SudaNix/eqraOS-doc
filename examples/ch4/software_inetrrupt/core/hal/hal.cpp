

#ifndef i386
#error "[hal.cpp] is not implemented in this platform"
#endif

#include <hal.h>
#include "cpu.h"
#include "idt.h"

extern int _cdecl hal_init() {
	i386_cpu_init();
	return 0;
}

extern int _cdecl hal_close() {
	i386_cpu_close();
	return 0;
}

extern void _cdecl gen_interrupt(int n) {
#ifdef _MSC_VER
	_asm {
		mov al, byte ptr [n]
		mov byte ptr [address+1], al
		jmp address
		
		address:
			int 0		// will execute int n.
	}
#endif
}