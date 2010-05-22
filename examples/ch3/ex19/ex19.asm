;****************************************************
; enable_a20_bios:
;		Enable A20 with BIOS int 0x15 routine 0x2401
;****************************************************

enable_a20_bios:

		pusha			; save all registers
		
		mov ax,0x2401	; Enable A20 routine.
		int 0x15
		
		popa			; restore registers
		ret
