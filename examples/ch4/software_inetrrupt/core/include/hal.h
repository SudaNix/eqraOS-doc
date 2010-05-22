#ifndef HAL_H
#define HAL_H

#ifndef i386
#error "HAL is not implemented in this platform"
#endif

#include <stdint.h>

#ifdef _MSC_VER
#define interrupt __declspec(naked)
#else
#define interrupt
#endif

#define far
#define near


/*	Interface	*/

extern int _cdecl hal_init();
extern int _cdecl hal_close();
extern void _cdecl gen_interrupt(int);


#endif // HAL_H