; Listing generated by Microsoft (R) Optimizing Compiler Version 14.00.50727.42 

	TITLE	c:\Documents and Settings\SudaNix\Desktop\docs\research\examples\ch4\ex4-1\core\kernel\kdisplay.cpp
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB LIBCMT
INCLUDELIB OLDNAMES

PUBLIC	?tbuf@@3PADA					; tbuf
PUBLIC	?bchars@@3PADA					; bchars
_BSS	SEGMENT
?tbuf@@3PADA DB	020H DUP (?)				; tbuf
__x_pos	DD	01H DUP (?)
__y_pos	DD	01H DUP (?)
__x_start DD	01H DUP (?)
__y_start DD	01H DUP (?)
__color	DD	01H DUP (?)
_BSS	ENDS
_DATA	SEGMENT
?bchars@@3PADA DB 030H					; bchars
	DB	031H
	DB	032H
	DB	033H
	DB	034H
	DB	035H
	DB	036H
	DB	037H
	DB	038H
	DB	039H
	DB	041H
	DB	042H
	DB	043H
	DB	044H
	DB	045H
	DB	046H
_DATA	ENDS
PUBLIC	?kputc@@YAXE@Z					; kputc
; Function compile flags: /Ogtpy
; File c:\documents and settings\sudanix\desktop\docs\research\examples\ch4\ex4-1\core\kernel\kdisplay.cpp
;	COMDAT ?kputc@@YAXE@Z
_TEXT	SEGMENT
_c$ = 8							; size = 1
?kputc@@YAXE@Z PROC					; kputc, COMDAT

; 18   : 	if (!c)

	mov	dl, BYTE PTR _c$[esp-4]
	test	dl, dl
	je	SHORT $LN5@kputc

; 19   : 		return;
; 20   : 		
; 21   : 	if (c == '\n' || c == '\r') {	/* start newline */

	cmp	dl, 10					; 0000000aH
	je	SHORT $LN2@kputc
	cmp	dl, 13					; 0000000dH
	je	SHORT $LN2@kputc

; 24   : 		return;
; 25   : 	}
; 26   : 	
; 27   : 	if ( _x_pos > 79 ) {

	mov	ecx, DWORD PTR __x_pos
	cmp	ecx, 79					; 0000004fH
	jbe	SHORT $LN1@kputc

; 28   : 		_y_pos += 2;
; 29   : 		_x_pos = _x_start;

	mov	eax, DWORD PTR __x_start
	add	DWORD PTR __y_pos, 2
	mov	DWORD PTR __x_pos, eax

; 39   : }

	ret	0
$LN1@kputc:

; 30   : 		return;
; 31   : 	}
; 32   : 	
; 33   : 	/* address to print character */
; 34   : 	unsigned char* p = (unsigned char*) VIDEO_MEMORY + (_x_pos++)*2 + _y_pos*80;

	mov	eax, DWORD PTR __y_pos
	lea	eax, DWORD PTR [eax+eax*4]
	lea	eax, DWORD PTR [ecx+eax*8]
	add	ecx, 1
	lea	eax, DWORD PTR [eax+eax+753664]
	mov	DWORD PTR __x_pos, ecx

; 35   : 	
; 36   : 	/* draw the character */
; 37   : 	*p++ = c;		// character.
; 38   : 	*p = _color;	// color.

	mov	cl, BYTE PTR __color
	mov	BYTE PTR [eax], dl
	mov	BYTE PTR [eax+1], cl

; 39   : }

	ret	0
$LN2@kputc:

; 22   : 		_y_pos += 2;
; 23   : 		_x_pos = _x_start;

	mov	edx, DWORD PTR __x_start
	add	DWORD PTR __y_pos, 2
	mov	DWORD PTR __x_pos, edx
$LN5@kputc:

; 39   : }

	ret	0
?kputc@@YAXE@Z ENDP					; kputc
_TEXT	ENDS
PUBLIC	?itoa@@YAXIIPAD@Z				; itoa
; Function compile flags: /Ogtpy
;	COMDAT ?itoa@@YAXIIPAD@Z
_TEXT	SEGMENT
_i$ = 8							; size = 4
_base$ = 12						; size = 4
_buf$ = 16						; size = 4
?itoa@@YAXIIPAD@Z PROC					; itoa, COMDAT

; 47   :    int pos = 0;
; 48   :    int opos = 0;
; 49   :    int top = 0;
; 50   : 
; 51   :    if (i == 0 || base > 16) {

	mov	eax, DWORD PTR _i$[esp-4]
	xor	ecx, ecx
	test	eax, eax
	push	esi
	je	SHORT $LN6@itoa
	mov	esi, DWORD PTR _base$[esp]
	cmp	esi, 16					; 00000010H
	ja	SHORT $LN6@itoa
$LL5@itoa:

; 54   :       return;
; 55   :    }
; 56   : 
; 57   :    while (i != 0) {
; 58   :       tbuf[pos] = bchars[i % base];

	xor	edx, edx
	div	esi

; 59   :       pos++;

	add	ecx, 1
	test	eax, eax
	mov	dl, BYTE PTR ?bchars@@3PADA[edx]
	mov	BYTE PTR ?tbuf@@3PADA[ecx-1], dl
	jne	SHORT $LL5@itoa

; 60   :       i /= base;
; 61   :    }
; 62   :    top=pos--;

	mov	esi, ecx
	sub	ecx, 1

; 63   :    for (opos=0; opos<top; pos--,opos++) {

	test	esi, esi
	push	edi
	mov	edi, DWORD PTR _buf$[esp+4]
	jle	SHORT $LN1@itoa

; 60   :       i /= base;
; 61   :    }
; 62   :    top=pos--;

	lea	ecx, DWORD PTR ?tbuf@@3PADA[ecx]
	npad	1
$LL3@itoa:

; 64   :       buf[opos] = tbuf[pos];

	mov	dl, BYTE PTR [ecx]
	mov	BYTE PTR [eax+edi], dl
	add	eax, 1
	sub	ecx, 1
	cmp	eax, esi
	jl	SHORT $LL3@itoa
$LN1@itoa:

; 65   :    }
; 66   :    buf[opos] = 0;

	mov	BYTE PTR [eax+edi], 0
	pop	edi
	pop	esi

; 67   : }

	ret	0
$LN6@itoa:

; 52   :       buf[0] = '0';

	mov	eax, DWORD PTR _buf$[esp]
	mov	BYTE PTR [eax], 48			; 00000030H

; 53   :       buf[1] = '\0';

	mov	BYTE PTR [eax+1], cl
	pop	esi

; 67   : }

	ret	0
?itoa@@YAXIIPAD@Z ENDP					; itoa
_TEXT	ENDS
PUBLIC	?itoa_s@@YAXHIPAD@Z				; itoa_s
; Function compile flags: /Ogtpy
;	COMDAT ?itoa_s@@YAXHIPAD@Z
_TEXT	SEGMENT
_i$ = 8							; size = 4
_base$ = 12						; size = 4
_buf$ = 16						; size = 4
?itoa_s@@YAXHIPAD@Z PROC				; itoa_s, COMDAT

; 70   :    if (base > 16) return;

	mov	edx, DWORD PTR _base$[esp-4]
	cmp	edx, 16					; 00000010H
	ja	SHORT $LN3@itoa_s

; 71   :    if (i < 0) {

	mov	eax, DWORD PTR _i$[esp-4]
	test	eax, eax

; 72   :       *buf++ = '-';

	mov	ecx, DWORD PTR _buf$[esp-4]
	jge	SHORT $LN1@itoa_s
	mov	BYTE PTR [ecx], 45			; 0000002dH
	add	ecx, 1

; 73   :       i *= -1;

	neg	eax
$LN1@itoa_s:

; 74   :    }
; 75   :    itoa(i,base,buf);

	mov	DWORD PTR _buf$[esp-4], ecx
	mov	DWORD PTR _base$[esp-4], edx
	mov	DWORD PTR _i$[esp-4], eax
	jmp	?itoa@@YAXIIPAD@Z			; itoa
$LN3@itoa_s:

; 76   : }

	ret	0
?itoa_s@@YAXHIPAD@Z ENDP				; itoa_s
_TEXT	ENDS
PUBLIC	?kclear@@YAXG@Z					; kclear
; Function compile flags: /Ogtpy
;	COMDAT ?kclear@@YAXG@Z
_TEXT	SEGMENT
_c$ = 8							; size = 2
?kclear@@YAXG@Z PROC					; kclear, COMDAT

; 84   : 	unsigned char* p = (unsigned char*) VIDEO_MEMORY;
; 85   : 	
; 86   : 	for (int i=0;i<160*30;i+=2) {

	mov	dl, BYTE PTR _c$[esp-4]
	mov	eax, 753665				; 000b8001H
	mov	ecx, 2400				; 00000960H
	npad	2
$LL3@kclear:

; 87   : 		p[i] = ' ';

	mov	BYTE PTR [eax-1], 32			; 00000020H

; 88   : 		p[i+1] = c;

	mov	BYTE PTR [eax], dl
	add	eax, 2
	sub	ecx, 1
	jne	SHORT $LL3@kclear

; 89   : 	}
; 90   : 	
; 91   : 	_x_pos = _x_start;

	mov	eax, DWORD PTR __x_start

; 92   : 	_y_pos = _y_start;

	mov	ecx, DWORD PTR __y_start
	mov	DWORD PTR __x_pos, eax
	mov	DWORD PTR __y_pos, ecx

; 93   : }

	ret	0
?kclear@@YAXG@Z ENDP					; kclear
_TEXT	ENDS
PUBLIC	?kputs@@YAXPAD@Z				; kputs
EXTRN	?strlen@@YAIPBD@Z:PROC				; strlen
; Function compile flags: /Ogtpy
;	COMDAT ?kputs@@YAXPAD@Z
_TEXT	SEGMENT
_str$ = 8						; size = 4
?kputs@@YAXPAD@Z PROC					; kputs, COMDAT

; 97   : {

	push	edi

; 98   : 	if (!str)

	mov	edi, DWORD PTR _str$[esp]
	test	edi, edi
	je	SHORT $LN1@kputs
	push	esi

; 99   : 		return;
; 100  : 		
; 101  : 	for (size_t i=0;i<strlen(str);++i)

	push	edi
	xor	esi, esi
	call	?strlen@@YAIPBD@Z			; strlen
	add	esp, 4
	test	eax, eax
	jbe	SHORT $LN9@kputs
	npad	7
$LL3@kputs:

; 102  : 		kputc(str[i]);

	movzx	eax, BYTE PTR [esi+edi]
	push	eax
	call	?kputc@@YAXE@Z				; kputc
	push	edi
	add	esi, 1
	call	?strlen@@YAIPBD@Z			; strlen
	add	esp, 8
	cmp	esi, eax
	jb	SHORT $LL3@kputs
$LN9@kputs:
	pop	esi
$LN1@kputs:
	pop	edi

; 103  : }

	ret	0
?kputs@@YAXPAD@Z ENDP					; kputs
_TEXT	ENDS
PUBLIC	?ksetcolor@@YAII@Z				; ksetcolor
; Function compile flags: /Ogtpy
;	COMDAT ?ksetcolor@@YAII@Z
_TEXT	SEGMENT
_c$ = 8							; size = 4
?ksetcolor@@YAII@Z PROC					; ksetcolor, COMDAT

; 108  : 	unsigned old_color = _color;
; 109  : 	_color = c;

	mov	ecx, DWORD PTR _c$[esp-4]
	mov	eax, DWORD PTR __color
	mov	DWORD PTR __color, ecx

; 110  : 	return old_color;
; 111  : }

	ret	0
?ksetcolor@@YAII@Z ENDP					; ksetcolor
_TEXT	ENDS
PUBLIC	?kgoto_xy@@YAXII@Z				; kgoto_xy
; Function compile flags: /Ogtpy
;	COMDAT ?kgoto_xy@@YAXII@Z
_TEXT	SEGMENT
_x$ = 8							; size = 4
_y$ = 12						; size = 4
?kgoto_xy@@YAXII@Z PROC					; kgoto_xy, COMDAT

; 115  : 	_x_pos = x*2;

	mov	eax, DWORD PTR _x$[esp-4]

; 116  : 	_y_pos = y*2;

	mov	ecx, DWORD PTR _y$[esp-4]
	add	eax, eax
	add	ecx, ecx
	mov	DWORD PTR __x_pos, eax
	mov	DWORD PTR __y_pos, ecx

; 117  : 	_x_start = _x_pos;

	mov	DWORD PTR __x_start, eax

; 118  : 	_y_start = _y_pos;

	mov	DWORD PTR __y_start, ecx

; 119  : }

	ret	0
?kgoto_xy@@YAXII@Z ENDP					; kgoto_xy
_TEXT	ENDS
PUBLIC	?kprintf@@YAHPBDZZ				; kprintf
; Function compile flags: /Ogtpy
;	COMDAT ?kprintf@@YAHPBDZZ
_TEXT	SEGMENT
_str$2671 = -32						; size = 32
_str$2661 = -32						; size = 32
_str$2651 = -32						; size = 32
_str$ = 8						; size = 4
?kprintf@@YAHPBDZZ PROC					; kprintf, COMDAT

; 122  : int kprintf(const char* str, ...) {

	sub	esp, 32					; 00000020H
	push	ebx
	push	ebp

; 123  : 
; 124  : 	if(!str)

	mov	ebp, DWORD PTR _str$[esp+36]
	xor	ebx, ebx
	cmp	ebp, ebx
	jne	SHORT $LN15@kprintf
	pop	ebp

; 125  : 		return 0;

	xor	eax, eax
	pop	ebx

; 188  : 		}
; 189  : 
; 190  : 	}
; 191  : 
; 192  : 	va_end (args);
; 193  : }

	add	esp, 32					; 00000020H
	ret	0
$LN15@kprintf:
	push	edi

; 126  : 
; 127  : 	va_list		args;
; 128  : 	va_start (args, str);
; 129  : 
; 130  : 	for (size_t i=0; i<strlen(str);i++) {

	push	ebp
	xor	edi, edi
	call	?strlen@@YAIPBD@Z			; strlen
	add	esp, 4
	test	eax, eax
	jbe	$LN33@kprintf
	push	esi
	lea	esi, DWORD PTR _str$[esp+44]
$LL14@kprintf:

; 131  : 
; 132  : 		switch (str[i]) {

	movzx	eax, BYTE PTR [edi+ebp]
	cmp	al, 37					; 00000025H
	je	SHORT $LN9@kprintf

; 181  : 				}
; 182  : 
; 183  : 				break;
; 184  : 
; 185  : 			default:
; 186  : 				kputc (str[i]);

	push	eax
	call	?kputc@@YAXE@Z				; kputc
	add	esp, 4

; 187  : 				break;

	jmp	$LN13@kprintf
$LN9@kprintf:

; 133  : 
; 134  : 			case '%':
; 135  : 
; 136  : 				switch (str[i+1]) {

	movsx	eax, BYTE PTR [edi+ebp+1]
	add	eax, -88				; ffffffa8H
	cmp	eax, 32					; 00000020H
	ja	$LN2@kprintf
	movzx	eax, BYTE PTR $LN32@kprintf[eax]
	jmp	DWORD PTR $LN37@kprintf[eax*4]
$LN6@kprintf:

; 137  : 
; 138  : 					/*** characters ***/
; 139  : 					case 'c': {
; 140  : 						char c = va_arg (args, char);
; 141  : 						kputc (c);

	movzx	ecx, BYTE PTR [esi+4]
	add	esi, 4
	push	ecx
	call	?kputc@@YAXE@Z				; kputc
	add	esp, 4

; 142  : 						i++;		// go to next character
; 143  : 						break;

	jmp	$LN35@kprintf
$LN5@kprintf:

; 144  : 					}
; 145  : 
; 146  : 					/*** address of ***/
; 147  : 					case 's': {
; 148  : 						int c = (int&) va_arg (args, char);
; 149  : 						char str[32]={0};

	xor	eax, eax
	add	esi, 4
	mov	DWORD PTR _str$2651[esp+49], eax
	mov	DWORD PTR _str$2651[esp+53], eax
	mov	DWORD PTR _str$2651[esp+57], eax
	mov	DWORD PTR _str$2651[esp+61], eax
	mov	DWORD PTR _str$2651[esp+65], eax
	mov	DWORD PTR _str$2651[esp+69], eax
	mov	DWORD PTR _str$2651[esp+73], eax
	mov	WORD PTR _str$2651[esp+77], ax
	mov	BYTE PTR _str$2651[esp+79], al

; 150  : 						itoa_s (c, 16, str);

	mov	eax, DWORD PTR [esi]
	cmp	eax, ebx
	mov	BYTE PTR _str$2651[esp+48], bl
	lea	ecx, DWORD PTR _str$2651[esp+48]
	jge	SHORT $LN18@kprintf
	mov	BYTE PTR _str$2651[esp+48], 45		; 0000002dH
	lea	ecx, DWORD PTR _str$2651[esp+49]
	neg	eax
$LN18@kprintf:
	push	ecx
	push	16					; 00000010H
	push	eax
	call	?itoa@@YAXIIPAD@Z			; itoa

; 151  : 						kputs (str);

	lea	edx, DWORD PTR _str$2651[esp+60]
	push	edx

; 152  : 						i++;		// go to next character
; 153  : 						break;

	jmp	$LN36@kprintf
$LN4@kprintf:

; 154  : 					}
; 155  : 
; 156  : 					/*** integers ***/
; 157  : 					case 'd':
; 158  : 					case 'i': {
; 159  : 						int c = va_arg (args, int);
; 160  : 						char str[32]={0};

	xor	eax, eax
	add	esi, 4
	mov	DWORD PTR _str$2661[esp+49], eax
	mov	DWORD PTR _str$2661[esp+53], eax
	mov	DWORD PTR _str$2661[esp+57], eax
	mov	DWORD PTR _str$2661[esp+61], eax
	mov	DWORD PTR _str$2661[esp+65], eax
	mov	DWORD PTR _str$2661[esp+69], eax
	mov	DWORD PTR _str$2661[esp+73], eax
	mov	WORD PTR _str$2661[esp+77], ax
	mov	BYTE PTR _str$2661[esp+79], al

; 161  : 						itoa_s (c, 10, str);

	mov	eax, DWORD PTR [esi]
	cmp	eax, ebx
	mov	BYTE PTR _str$2661[esp+48], bl
	lea	ecx, DWORD PTR _str$2661[esp+48]
	jge	SHORT $LN22@kprintf
	mov	BYTE PTR _str$2661[esp+48], 45		; 0000002dH
	lea	ecx, DWORD PTR _str$2661[esp+49]
	neg	eax
$LN22@kprintf:
	push	ecx
	push	10					; 0000000aH
	push	eax
	call	?itoa@@YAXIIPAD@Z			; itoa

; 162  : 						kputs (str);

	lea	eax, DWORD PTR _str$2661[esp+60]
	push	eax

; 163  : 						i++;		// go to next character
; 164  : 						break;

	jmp	SHORT $LN36@kprintf
$LN3@kprintf:

; 165  : 					}
; 166  : 
; 167  : 					/*** display in hex ***/
; 168  : 					case 'X':
; 169  : 					case 'x': {
; 170  : 						int c = va_arg (args, int);
; 171  : 						char str[32]={0};

	xor	eax, eax
	add	esi, 4
	mov	DWORD PTR _str$2671[esp+49], eax
	mov	DWORD PTR _str$2671[esp+53], eax
	mov	DWORD PTR _str$2671[esp+57], eax
	mov	DWORD PTR _str$2671[esp+61], eax
	mov	DWORD PTR _str$2671[esp+65], eax
	mov	DWORD PTR _str$2671[esp+69], eax
	mov	DWORD PTR _str$2671[esp+73], eax
	mov	WORD PTR _str$2671[esp+77], ax
	mov	BYTE PTR _str$2671[esp+79], al

; 172  : 						itoa_s (c,16,str);

	mov	eax, DWORD PTR [esi]
	cmp	eax, ebx
	mov	BYTE PTR _str$2671[esp+48], bl
	lea	ecx, DWORD PTR _str$2671[esp+48]
	jge	SHORT $LN26@kprintf
	mov	BYTE PTR _str$2671[esp+48], 45		; 0000002dH
	lea	ecx, DWORD PTR _str$2671[esp+49]
	neg	eax
$LN26@kprintf:
	push	ecx
	push	16					; 00000010H
	push	eax
	call	?itoa@@YAXIIPAD@Z			; itoa

; 173  : 						kputs (str);

	lea	ecx, DWORD PTR _str$2671[esp+60]
	push	ecx
$LN36@kprintf:
	call	?kputs@@YAXPAD@Z			; kputs
	add	esp, 16					; 00000010H
$LN35@kprintf:

; 174  : 						i++;		// go to next character

	add	edi, 1
$LN13@kprintf:

; 126  : 
; 127  : 	va_list		args;
; 128  : 	va_start (args, str);
; 129  : 
; 130  : 	for (size_t i=0; i<strlen(str);i++) {

	push	ebp
	add	edi, 1
	call	?strlen@@YAIPBD@Z			; strlen
	add	esp, 4
	cmp	edi, eax
	jb	$LL14@kprintf
	pop	esi
	pop	edi
	pop	ebp
	pop	ebx

; 188  : 		}
; 189  : 
; 190  : 	}
; 191  : 
; 192  : 	va_end (args);
; 193  : }

	add	esp, 32					; 00000020H
	ret	0
$LN2@kprintf:

; 175  : 						break;
; 176  : 					}
; 177  : 
; 178  : 					default:
; 179  : 						va_end (args);
; 180  : 						return 1;

	mov	eax, 1
	pop	esi
$LN33@kprintf:
	pop	edi
	pop	ebp
	pop	ebx

; 188  : 		}
; 189  : 
; 190  : 	}
; 191  : 
; 192  : 	va_end (args);
; 193  : }

	add	esp, 32					; 00000020H
	ret	0
$LN37@kprintf:
	DD	$LN3@kprintf
	DD	$LN6@kprintf
	DD	$LN4@kprintf
	DD	$LN5@kprintf
	DD	$LN2@kprintf
$LN32@kprintf:
	DB	0
	DB	4
	DB	4
	DB	4
	DB	4
	DB	4
	DB	4
	DB	4
	DB	4
	DB	4
	DB	4
	DB	1
	DB	2
	DB	4
	DB	4
	DB	4
	DB	4
	DB	2
	DB	4
	DB	4
	DB	4
	DB	4
	DB	4
	DB	4
	DB	4
	DB	4
	DB	4
	DB	3
	DB	4
	DB	4
	DB	4
	DB	4
	DB	0
?kprintf@@YAHPBDZZ ENDP					; kprintf
_TEXT	ENDS
END