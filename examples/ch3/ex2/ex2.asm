;Hello Bootloader

bits 16				; 16-bit real mode.
org	 0x0			; this number will added to all addresses (relocating).

start:
		jmp main	; jump over data and function to entru point.


		
; ****************
;	data
; ****************

hello_msg		db		"Welcome to eqraOS, Coded by Ahmad Essam",0xa,0xd,0

; *********************************************
;	puts16: prints string using BIOS interrupt
;   input:
;				ds: pointer to data segment.
;				si: point to the string
; *********************************************

puts16:
		
		lodsb			; read character from ds:si to al ,and increment si if df=0.
		
		cmp al,0		; check end of string ?
		je end_puts16	; yes jump to end.
		
		mov ah,0xe		; print character routine number.
		int 0x10		; call BIOS.
		
		jmp puts16		; continue prints until 0 is found.
		
	end_puts16:
	
		ret				
		
		
; ***************************************
;		entry point of bootloader.
; ***************************************
		
main:				

		;---------------------
		;	intit registers
		;---------------------
		
		; because bootloader are loaded at 0x07c00 we can refrence this location with many different combination
		; of segment:offset addressing.
		
		; So we will use either 0x0000:0x7c000 or 0x:07c0:0x0000
		; and in this example we use 0x07c0 for segment and 0x0 for offset.
		
		mov ax,0x07c0			
		mov ds,ax
		mov es,ax
		
		mov si,hello_msg
		call puts16

		cli			; clear interrupt.		
		hlt			; halt the system.
		
		times	510-($-$$)	db		0		; append zeros.
		
		; finally the boot signature 0xaa55
		db		0x55
		db		0xaa
		