
#ifndef NULL_H
#define NULL_H

#if define (_MSC_VER) && (_MSC_VER > = 1020)
#pargma once
#endif

#ifdef NULL
#undev NULL
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* C++ NULL definition */
#define NULL 0

#ifdef __cplusplus
}
#else

/* C NULL definition */
#define NULL (void*)0

#endif



#endif //NULL_H