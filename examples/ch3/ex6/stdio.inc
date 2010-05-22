; stdio.inc: 16/32 bit Input/Output Routines.

%ifndef STDIO_INC
%define STDIO_INC

; *********************************************
;	puts16: prints string using BIOS interrupt
;   input:
;				ds: pointer to data segment.
;				si: point to the string
; *********************************************
bits	16

puts16:
		pusha			; save registers
		
		lodsb			; read character from ds:si to al ,and increment si if df=0.
		
		cmp al,0		; check end of string ?
		je end_puts16	; yes jump to end.
		
		mov ah,0xe		; print character routine number.
		int 0x10		; call BIOS.
		
		jmp puts16		; continue prints until 0 is found.
		
	end_puts16:
	
		popa			; restore registers
		ret		

%endif STDIO_INC