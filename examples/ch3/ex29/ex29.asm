

bits 16		; 16-bit real mode.
org 0x500

start:	jmp stage2

;**************************
; include files:
;**************************
%include "stdio.inc"		; standard I/O routines.
%include "gdt.inc"			; GDT load routine.
%include "a20.inc"			; Enable A20 routines.
%include "fat12.inc"		; FAT12 driver.
%include "common.inc"		; common declarations.

;*********************
; data and variable
;*********************

hello_msg		db		0xa,0xd,"Welcome to eqraOS Stage2",0xa,0xd,0
fail_message	db		0xa,0xd,"KERNEL.SYS is Missing. press any key to reboot...",0



; ***************************************
;	entry point of stage2 bootloader.
; ***************************************

stage2:
	
	;---------------
	; Set Registers.
	;---------------

		cli				
		
		xor	ax, ax			
		mov	ds, ax
		mov	es, ax
		
		mov	ax, 0x0		
		mov	ss, ax
		mov	sp, 0xFFFF
		
		sti			
	
	;--------------------
	; Load gdt into gdtr.
	;--------------------
	
		call load_gdt
		
	;--------------------
	; Enable A20.
	;--------------------
		call enable_a20_keyboard_controller_output_port
	
	;----------------
	; Display Message.
	;-----------------
		mov si,hello_msg
		call puts16
		
	;---------------------
	; Load Root Directory
	;----------------------
		call load_root
		
	;---------------------
	; Load Kernel
	;---------------------
		xor ebx,ebx
		mov bp,KERNEL_RMODE_BASE		; bx:bp buffer to load kernel
		
		mov si,kernel_name
		call load_file
		
		mov dword[kernel_size],ecx
		cmp ax,0
		je enter_stage3
		
		mov si,fail_message
		call puts16
		
		mov ah,0
		int 0x16			; wait any key.
		int 0x19			; warm boot.
		cli					; cannot go here!
		hlt
		
		
	;----------------
	; Go to PMode.
	;----------------
	
	enter_stage3:
	
		; just set bit 0 from cr0 (Control Register 0).
		
		cli					; important.
		mov eax,cr0			
		or eax,0x1
		mov cr0,eax			; entering pmode.
		
		
	;-------------------
	; Fix CS value
	;--------------------
		; select the code descriptor
		jmp CODE_DESCRIPTOR:stage3
		
		
;**************************
;	entry point of stage3
;**************************		

bits 32			; code now 32-bit

stage3:
		
	;----------------;
	; Set Registers.
	;----------------;
		
		mov ax,DATA_DESCRIPTOR			; address of data descriptor.
		mov ds,ax
		mov ss,ax
		mov es,ax
		mov esp,0x90000		; stack begin from 0x90000.
		
	;---------------------------------
	; Clear Screen and print message.
	;---------------------------------
	
		call clear_screen
		
		mov ebx,stage2_message
		call puts32
		
		mov ebx,logo_message
		call puts32
		
		
	
	;------------------------
	; Copy Kernel at 1 MB.
	;-----------------------
		mov eax,dword[kernel_size]
		movzx ebx,word[bytes_per_sector]
		mul ebx
		mov ebx,4
		div ebx
		
		cld
		
		mov esi,KERNEL_RMODE_BASE
		mov edi,KERNEL_PMODE_BASE
		mov ecx,eax
		rep movsd
		
	;--------------------
	; Execute the kernel.
	;--------------------
		jmp CODE_DESCRIPTOR:KERNEL_PMODE_BASE
		
	;-----------------;
	; Hlat the system.
	;-----------------;
		cli			; clear interrupt.		
		hlt			; halt the system.
		

