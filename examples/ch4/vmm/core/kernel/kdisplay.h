#ifndef KDISPLAY_H
#define KDISPLAY_H

extern void kclear(const uint8_t c);
extern void kputs(char* str);
extern void kputc(unsigned char c);
extern int kprintf(const char* str,...);
extern unsigned kset_color(const unsigned c);
extern void kgoto_xy(unsigned x,unsigned y);

#endif // KDISPLAY_H