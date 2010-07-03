#include <string.h>

#pragma warning (disable:4706)

int strcmp(const char* str1, const char* str2) {

	int res=0;
	while (!(res = *(unsigned char*)str1 - *(unsigned char*)str2) && *str2)
		++str1, ++str2;

	if (res < 0)
		res = -1;
	if (res > 0)
		res = 1;

	return res;
}

size_t strlen(const char* str) {
	size_t length = 0;
	while (str[length++]);
	return length;
}

char* strcpy(char* s1,const char* s2) {
	char* p = s1;
	while ( *s1++ = *s2++ );
	return p;
}

void* memcpy(void* dest,const void* src,size_t count) {
	const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void* memset(void* dest,char val,size_t count) {
    unsigned char *temp = (unsigned char *)dest;
	for( ; count != 0; count--, temp[count] = val);
	return dest;
}

unsigned short* memsetw(unsigned short* dest,unsigned short val,size_t count) {
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--)
		*temp++ = val;
    return dest;
}
