
; *********************************************
;	putch32: print character in protected mode.
;   input:
;				bl: character to print.
; *********************************************

bits	32

%define VIDEO_MEMORY	0xb8000		; Base Address of Mapped Video Memory.
%define COLUMNS			80			; text mode (mode 7) has 80 columns,
%define	ROWS			25			; and 25 rows.
%define CHAR_ATTRIBUTE	31			; white on blue.

x_pos		db		0			; current x position.
y_pos		db		0			; current y position.

putch32:

		pusha			; Save Registers.
		
	;---------------------------
	; Check if bl is new line ?
	;---------------------------
	
		cmp bl,0xa		; if character is newline ?
		je new_row		; yes, jmp at end.
	
	;------------------------------
	; Calculate the memory offset 
	;-----------------------------
		; because in text mode every character take 2 bytes: one for the character and one for the attribute, we must calculate the memory offset with the follwing formula:
		; offset = x_pos*2 + y_pos*COLUMNS*2
		
		xor eax,eax
		
		mov al,2
		mul byte[x_pos]
		push eax			; save the first section of formula.
		
		xor eax,eax
		xor ecx,ecx
		
		mov ax,COLUMNS*2		; 80*2
		mov cl,byte[y_pos]
		mul ecx
		
		pop ecx
		add eax,ecx			
		
		add eax,VIDEO_MEMORY	; eax = address to print the character.
		
	;---------------------
	; Print the chracter.
	;---------------------
		
		mov edi,eax
		
		mov byte[edi],bl					; print the character,
		mov byte[edi+1],CHAR_ATTRIBUTE		; with respect to the attribute.
		
	;---------------------
	; Update the postions.
	;---------------------
		
		inc byte[x_pos]
		cmp byte[x_pos],COLUMNS
		je new_row
		
		jmp putch32_end
		
		
	new_row:
		
		mov byte[x_pos],0				; clear the x_pos.
		inc byte[y_pos]					; increment the y_pos.
		
	putch32_end:	
		
		popa			; Restore Registers.
		
		ret

		