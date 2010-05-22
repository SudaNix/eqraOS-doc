

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
		mov bl,byte[edi]	; read character.
		
		cmp bl,0x0			; end of string ?
		je	puts32_end		; yes, jmp to end.
		
		call putch32		; print the character.
		
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
