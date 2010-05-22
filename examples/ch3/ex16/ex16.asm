
bits	16		; real mode.

;******************************
; load_gdt: Load GDT into GDTR.
;******************************

load_gdt:
		
		cli					; clear interrupt.
		pusha				; save registers
		lgdt [gdt_ptr]		; load gdt into gdtr
		sti					; enable interrupt
		popa				; restore registers.
		
		ret


;**************************************
; gdt_ptr: data structure used by gdtr
;**************************************

gdt_ptr:
		
		dw end_of_gdt - begin_of_gdt - 1		; size -1
		dd begin_of_gdt							; base of gdt
		