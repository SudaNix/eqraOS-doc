;***************************
; Global Descriptor Table
;***************************

begin_of_gdt:

; Null Descriptor: start at 0x0.

	dd	0x0			; fill 8 byte with zero.
	dd	0x0
	
; Code Descriptor: start at 0x8.

	dw	0xffff		; limit low.
	dw 	0x0			; base low.
	db	0x0			; base middle.
	db	10011010b	; access byte.
	db	11001111b	; granularity byte.
	db	0x0			; base high.

; Data Descriptor: start at 0x10.

	dw	0xffff		; limit low.
	dw 	0x0			; base low.
	db	0x0			; base middle.
	db	10010010b	; access byte.
	db	11001111b	; granularity byte.
	db	0x0			; base high.
	
end_of_gdt: