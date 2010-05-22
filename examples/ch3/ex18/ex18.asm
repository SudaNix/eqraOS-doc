;*************************************************
; enable_a20_port_0x92:
;		Enable A20 with System Control port 0x92
;*************************************************

enable_a20_port_0x92:

		push ax			; save register.
		
		mov al,2		; set bit 2 to enable A20
		out 0x92,al
		
		pop ax			; restore register.
		ret