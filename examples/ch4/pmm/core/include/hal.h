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
extern void _cdecl int_done(unsigned int n);
extern void _cdecl sound(unsigned int f);
extern unsigned char _cdecl inportb(unsigned short port_num);
extern void _cdecl outportb(unsigned short port_num,unsigned char value);
extern void _cdecl enable_irq();
extern void _cdecl disable_irq();
extern void _cdecl set_vector(unsigned int int_num,void (_cdecl far & vect)());
extern void (_cdecl far * _cdecl get_vector(unsigned int int_num))();
extern const char* _cdecl get_cpu_vendor();
extern int _cdecl get_tick_count();

#endif // HAL_H