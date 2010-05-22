
;****************************************************
; enable_a20_keyboard_controller:
;		Enable A20 with command 0xdd
;****************************************************

enable_a20_keyboard_controller:

		;cli
		push ax			; save register.
		
		mov al,0xdd		; Enable A20 Keyboard Controller Command.
		out 0x64,al
		
		pop ax			; restore register.
		ret
		