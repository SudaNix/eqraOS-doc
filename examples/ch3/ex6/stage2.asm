 ; Second Stage Bootloader.
 ; loaded by stage1.bin at address 0x050:0x0 (0x00500).
 

bits 16		; 16-bit real mode.
org  0x0    ; offset to zero.

start:	jmp stage2


; data and variable
hello_msg	db		"Welcome to eqraOS Stage2",0xa,0xd,0

; include files:
%include "stdio.inc"		; standard i/o routines.


		

; ***************************************
;	entry point of stage2 bootloader.
; ***************************************

stage2:
		
		push cs
		pop ds		; ds = cs.
		
		mov si,hello_msg
		call puts16
		
		cli			; clear interrupt.		
		hlt			; halt the system.
		
		
