/*
 Kernel entry point.
 Called directly from bootloader.
 
  eqraOS project,Copyright (c) 2010 Ahmad Essam.
			suda[DOT]nix[AT]hotmail[DOT]com
*/

#include <multiboot_info.h>
extern int _cdecl main (multiboot_info* boot_info);			// main function.
extern void _cdecl init_ctor();		// init constructor.
extern void _cdecl exit ();  		// exit.

void _cdecl kernel_entry (multiboot_info* boot_info)
{

#ifdef i386
	_asm {
		cli						
		
		mov ax, 10h				// select data descriptor in GDT.
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
	}
#endif

	// Execute global constructors
	init_ctor();

	// Call kernel entry point
	main(boot_info);

	// Cleanup all dynamic dtors
	exit();

#ifdef i386
	_asm {
		cli
		hlt
	}
#endif

	for(;;);
}
