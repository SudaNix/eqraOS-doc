
; ***********************************************
;	clear_screen: Clear Screen in protected mode.
; ***********************************************

bits	32

clear_screen:

		pusha			; Save Registers.
		cld
		
		mov edi,VIDEO_MEMORY		; base address of video memory.
		mov cx,2000		; 25*80
		mov ah,CHAR_ATTRIBUTE		; 31 = white character on blue background.
		mov al,' '
		
		rep stosw
		
		mov byte[x_pos],0
		mov byte[y_pos],0
		
		popa			; Restore Registers.
		
		ret
		