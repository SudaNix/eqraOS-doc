
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
		mov ebx,eax
		
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
