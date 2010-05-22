#ifndef CTYPE_H
#define CTYPE_H

#ifdef _MSC_VER
#pragma warning (disable:4244)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

extern char _ctype[];

/* constants */

#define	CT_UP 		0x01	// upper case
#define CT_LOW		0x02	// lower case
#define CT_DIG		0x04	// digit
#define CT_CTL		0x08	// control
#define CT_PUN		0x10	// punctuation
#define CT_WHT		0x20	// white space (space,cr,lf,tab).
#define CT_HEX		0x40	// hex digit
#define CT_SP		0x80	// sapce.

/* macros */

#define isalnum(c)		( (_ctype+1)[(unsigned)(c)] & (CT_UP|CT_LOW|CT_DIG) )


// to be continue..

#ifdef __cplusplus
}
#endif

#endif // CTYPE_H