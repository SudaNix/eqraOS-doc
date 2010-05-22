
;*******************************************
; wait_output: wait output buffer to be full.
;*******************************************

wait_output:

		in al,0x64		; read status
		test al,0x1		; is output buffer is empty?
		je wait_output	; yes, hang.
		
		ret				; no,there is a result.
		
		
;******************************************
; wait_input: wait input buffer to be empty.
				command executed already.
;******************************************

wait_input:
	
		in al,0x64		; read status
		test al,0x2		; is input buffer is full?
		jne wait_input	; yes, hang.
		
		ret				; no,command executed.
		