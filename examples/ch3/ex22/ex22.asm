
;****************************************************
; enable_a20_keyboard_controller_output_port:
;		Enable A20 with write to keyboard output port.
;****************************************************

enable_a20_keyboard_controller_output_port:

		cli
		pusha			; save all registers
		
		call wait_input		; wait last operation to be finished.
		
		;------------------
		; Disable Keyboard
		;------------------
		mov al,0xad		; disable keyboard command.
		out 0x64,al
		call wait_input
		
		;-------------------------------
		; send read output port command
		;--------------------------------
		mov al,0xd0		; read output port command
		out 0x64,al
		call wait_output		; wait output to come.
		; we don't need to wait_input bescause when output came we know that operation are executed.
		
		;----------------------
		; read input buffer
		;----------------------
		in al,0x60
		push eax		; save data.
		call wait_input
		
		;--------------------------------
		; send write output port command.
		;--------------------------------
		mov al,0xd1		; write output port command.
		out 0x64,al
		call wait_input
		
		;------------
		; enable a20.
		;------------
		pop eax
		or al,2			; set bit 2.
		out 0x60,al
		call wait_input
		
		;-----------------
		; Enable Keyboard.
		;-----------------
		mov al,0xae		; Enable Keyboard command.
		out 0x64,al
		call wait_input
		
		
		popa			; restore registers
		sti
		
		ret
		