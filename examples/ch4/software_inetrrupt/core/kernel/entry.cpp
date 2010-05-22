/*
 Kernel entry point.
 Called directly from bootloader.
 
  eqraOS project,Copyright (c) 2010 Ahmad Essam.
			suda[DOT]nix[AT]hotmail[DOT]com
*/

extern int _cdecl main ();			// main function.
extern void _cdecl init_ctor();		// init constructor.
extern void _cdecl exit ();  		// exit.

void _cdecl kernel_entry ()
{

#ifdef i386
	_asm {
		cli						
		
		mov ax, 10h				// select data descriptor in GDT.
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		//mov ss, ax				// Set up base stack
		//mov esp, 0x90000
		//mov ebp, esp			// store current stack pointer
		//push ebp
	}
#endif

	// Execute global constructors
	init_ctor();

	// Call kernel entry point
	main();

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
