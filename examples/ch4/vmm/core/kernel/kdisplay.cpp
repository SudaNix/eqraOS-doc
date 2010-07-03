/* Modified Code from brokenthorn.com*/

#include <stdarg.h>
#include <string.h>
#include <stdint.h>

#include "kdisplay.h"

uint16_t *video_memory = (uint16_t *)0xb8000;

uint8_t _x_pos = 0, _y_pos =0;
uint8_t _color =0;
//static unsigned int _x_start = 0, _y_start =0;


#ifdef _MSC_VER
#pragma warning(disable:4244)
#endif

void k_update_cursor (int x, int y) {

    // get location
    uint16_t cursorLocation = y * 80 + x;

#if 0
	// send location to vga controller to set cursor
	disable();
    outportb(0x3D4, 14);
    outportb(0x3D5, cursorLocation >> 8); // Send the high byte.
    outportb(0x3D4, 15);
    outportb(0x3D5, cursorLocation);      // Send the low byte.
	enable();
#endif

}

void kputc(unsigned char c)
{
    uint16_t attribute = _color << 8;

    //! backspace character
    if (c == 0x08 && _x_pos)
        _x_pos--;

    //! tab character
    else if (c == 0x09)
        _x_pos = (_x_pos+8) & ~(8-1);

    //! carriage return
    else if (c == '\r')
        _x_pos = 0;

    //! new line
	else if (c == '\n') {
        _x_pos = 0;
        _y_pos++;
	}

    //! printable characters
    else if(c >= ' ') {

		//! display character on screen
        uint16_t* location = video_memory + (_y_pos*80 + _x_pos);
        *location = c | attribute;
        _x_pos++;
    }

    //! if we are at edge of row, go to new line
    if (_x_pos >= 80) {

        _x_pos = 0;
        _y_pos++;
    }

    //! update hardware cursor
	k_update_cursor (_x_pos,_y_pos);
}



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



/* Extern Function */

void kclear(const uint8_t c)
{
	//! clear video memory by writing space characters to it
	for (int i = 0; i < 80*25; i++)
        video_memory[i] = ' ' | (c << 8);

    //! move position back to start
    kgoto_xy (0,0);
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
	if (_x_pos <= 80)
	    _x_pos = x;

	if (_y_pos <= 25)
	    _y_pos = y;

	//! update hardware cursor to new position
	k_update_cursor(_x_pos, _y_pos);
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
						char str[64];
						strcpy (str,(const char*)c);
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
