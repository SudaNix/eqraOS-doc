#ifndef STDARG_H
#define STDARG_H

#include <va_list.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define STACK_ITEM int

#define VA_SIZE(TYPE)	\
		( (sizeof(TYPE) + sizeof(STACK_ITEM) -1 )	\
		& ~( sizeof(STACK_ITEM) -1) )
		
#define va_start(AP,LAST_ARG)	\
	(AP = ((va_list)&(LAST_ARG) + VA_SIZE(LAST_ARG)))
	
#define va_end(AP)

#define va_arg(AP,TYPE)	\
	(AP += VA_SIZE(TYPE), *((TYPE*)(AP - VA_SIZE(TYPE))))


	
#ifdef __cplusplus
}
#endif

#endif //STDARG_H