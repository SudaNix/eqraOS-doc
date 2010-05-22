	;--------------------
	; Load gdt into gdtr.
	;--------------------
	
		call load_gdt
		
	;----------------
	; Go to PMode.
	;----------------
		; just set bit 0 from cr0 (Control Register 0).
		
		cli					; important.
		mov eax,cr0			
		or eax,0x1
		mov cr0,eax			; entering pmode.
		
		
	;-------------------
	; Fix CS value
	;--------------------
		; select the code descriptor
		jmp 0x8:stage3
		
		
;**************************
;	entry point of stage3
;**************************		

bits 32			; code now 32-bit

stage3:
		
	;----------------;
	; Set Registers.
	;----------------;
		
		mov ax,0x10			; address of data descriptor.
		mov ds,ax
		mov ss,ax
		mov es,ax
		mov esp,0x90000		; stack begin from 0x90000.
		
		
	;-----------------;
	; Hlat the system.
	;-----------------;
		cli			; clear interrupt.		
		hlt			; halt the system.