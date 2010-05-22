#include <stdarg.h>
#include <string.h>

#include "kdisplay.h"

#define VIDEO_MEMORY	0xb8000

static unsigned int _x_pos = 0, _y_pos =0;
static unsigned int _x_start = 0, _y_start =0;
static unsigned int _color =0;

#ifdef _MSC_VER
#pragma warning(disable:4244)
#endif

void kputc(unsigned char c)
{
	if (!c)
		return;
		
	if (c == '\n' || c == '\r') {	/* start newline */
		_y_pos += 2;
		_x_pos = _x_start;
		return;
	}
	
	if ( _x_pos > 79 ) {
		_y_pos += 2;
		_x_pos = _x_start;
		return;
	}
	
	/* address to print character */
	unsigned char* p = (unsigned char*) VIDEO_MEMORY + (_x_pos++)*2 + _y_pos*80;
	
	/* draw the character */
	*p++ = c;		// character.
	*p = _color;	// color.
}


/* copied from brokenthorn.com*/
char tbuf[32];
char bchars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void itoa(unsigned i,unsigned base,char* buf) {
   int pos = 0;
   int opos = 0;
   int top = 0;

   if (i == 0 || base > 16) {
      buf[0] = '0';
      buf[1] = '\0';
      return;
   }

   while (i != 0) {
      tbuf[pos] = bchars[i % base];
      pos++;
      i /= base;
   }
   top=pos--;
   for (opos=0; opos<top; pos--,opos++) {
      buf[opos] = tbuf[pos];
   }
   buf[opos] = 0;
}

void itoa_s(int i,unsigned base,char* buf) {
   if (base > 16) return;
   if (i < 0) {
      *buf++ = '-';
      i *= -1;
   }
   itoa(i,base,buf);
}
/* end of copy */


/* Extern Function */

void kclear(const unsigned short c)
{
	unsigned char* p = (unsigned char*) VIDEO_MEMORY;
	
	for (int i=0;i<160*30;i+=2) {
		p[i] = ' ';
		p[i+1] = c;
	}
	
	_x_pos = _x_start;
	_y_pos = _y_start;
}


void kputs(char* str)
{
	if (!str)
		return;
		
	for (size_t i=0;i<strlen(str);++i)
		kputc(str[i]);
}


unsigned kset_color(const unsigned c)
{
	unsigned old_color = _color;
	_color = c;
	return old_color;
}

void kgoto_xy(unsigned x,unsigned y)
{
	_x_pos = x*2;
	_y_pos = y*2;
	_x_start = _x_pos;
	_y_start = _y_pos;
}


int kprintf(const char* str, ...) {

	if(!str)
		return 0;

	va_list		args;
	va_start (args, str);
	
	size_t i=0;

	for (; i<strlen(str);i++) {

		switch (str[i]) {

			case '%':

				switch (str[i+1]) {

					/*** characters ***/
					case 'c': {
						char c = va_arg (args, char);
						kputc (c);
						i++;		// go to next character
						break;
					}

					/*** address of ***/
					case 's': {
						int c = (int&) va_arg (args, char);
						char str[32]={0};
						itoa_s (c, 16, str);
						kputs (str);
						i++;		// go to next character
						break;
					}

					/*** integers ***/
					case 'd':
					case 'i': {
						int c = va_arg (args, int);
						char str[32]={0};
						itoa_s (c, 10, str);
						kputs (str);
						i++;		// go to next character
						break;
					}

					/*** display in hex ***/
					case 'X':
					case 'x': {
						int c = va_arg (args, int);
						char str[32]={0};
						itoa_s (c,16,str);
						kputs (str);
						i++;		// go to next character
						break;
					}

					default:
						va_end (args);
						return 1;
				}

				break;

			default:
				kputc (str[i]);
				break;
		}

	}

	va_end (args);
	return i;
}
