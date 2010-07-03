#include <hal.h>
#include <stdarg.h>
#include "kdisplay.h"

void _cdecl kernel_panic(const char* msg,...) {
	
	disable_irq();
	
	va_list args;
	va_start(args,msg);
	/* missing	*/
	va_end(args);
	
	char* panic = "\nSorry, eqraOS has encountered a problem and has been shutdown.\n\n";
	
	kclear(0x1f);
	kgoto_xy(0,0);
	kset_color(0x1f);
	kputs(panic);
	kprintf("*** STOP: %s",msg);
	
	/* hang	*/
	for (;;) ;
}
