; stdio.inc: 16/32 bit Input/Output Routines.
;
; eqraOS project,Copyright (c) 2010 Ahmad Essam.
;			suda[DOT]nix[AT]hotmail[DOT]com
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of version 2 of the GNU General Public License
; as published by the Free Software Foundation.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License along
; with this program; if not, write to the Free Software Foundation, Inc.,
; 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 
%ifndef STDIO_INC
%define STDIO_INC



; *********************************************
;	puts16: print string using BIOS interrupt
;   input:
;				ds: pointer to data segment.
;				si: point to the string
; *********************************************
bits	16

puts16:
		pusha			; save registers
	
	begin_puts16:
	
		lodsb			; read character from ds:si to al ,and increment si if df=0.
		
		cmp al,0		; check end of string ?
		je end_puts16	; yes jump to end.
		
		mov ah,0xe		; print character routine number.
		int 0x10		; call BIOS.
		
		jmp begin_puts16		; continue prints until 0 is found.
		
	end_puts16:
	
		popa			; restore registers
		ret		

		
		
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
		mul cx
		
		pop ecx
		add eax,ecx			
		
		add eax,VIDEO_MEMORY
		
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

; *********************************************
;	puts32: print string in protected mode.
;   input:
;				ebx: point to the string
; *********************************************

bits	32

puts32:

		pusha			; Save Registers.
		
		mov edi,ebx
		
	@loop:
		mov bl,byte[edi]
		
		cmp bl,0x0			; end of string ?
		je	puts32_end		; yes, jmp to end.
		
		call putch32
		
		inc edi				; point to the next character.
		
		jmp @loop
	
	puts32_end:
	
	;----------------------------
	; Update the Hardware Cursor.
	;-----------------------------
		; After print the string update the hardware cursor.
		
		mov bl,byte[x_pos]
		mov bh,byte[y_pos]
		
		call move_cursor
	
		popa			; Restore Registers.
		
		ret

		

; ******************************************
;	move_cursor: Move the Hardware Cursor.
;   input:
;				bl: x pos.
;				bh: y pos.
; ******************************************

bits	32
		
move_cursor:

		pusha			; Save Registers.
		
	;----------------------
	; Calculate the offset.
	;----------------------
		; offset = x_pos + y_pos*COLUMNS
		
		xor ecx,ecx
		mov cl,byte[x_pos]
		
		mov eax,COLUMNS
		mul byte[y_pos]
		
		add eax,ecx
		mov ebx,eax			; now, bl = low byte and bh = hight byte.
		
	;--------------------
	; Cursor Location Low.
	;---------------------
		
		mov al,0xf
		mov dx,0x3d4
		out dx,al
		
		mov al,bl
		mov dx,0x3d5
		out dx,al
		
	;----------------------
	; Cursor Location High.
	;----------------------
		
		mov al,0xe
		mov dx,0x3d4
		out dx,al
		
		mov al,bh
		mov dx,0x3d5
		out dx,al
		
		
		popa			; Restore Registers.
		
		ret

; ***********************************************
;	clear_screen: Clear Screen in protected mode.
; ***********************************************

bits	32

clear_screen:

		pusha			; Save Registers.
		cld
		
		mov edi,VIDEO_MEMORY
		mov cx,2000		; 25*80
		mov ah,CHAR_ATTRIBUTE
		mov al,' '
		
		rep stosw
		
		mov byte[x_pos],0
		mov byte[y_pos],0
		
		popa			; Restore Registers.
		
		ret
		
%endif ; STDIO_INC
