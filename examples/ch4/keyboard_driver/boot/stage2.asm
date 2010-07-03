; Second Stage Bootloader.
; loaded by stage1.bin at address 0x050:0x0 (0x00500).
;
; eqraOS project,Copyright (c) 2010 Ahmad Essam.
;			suda[DOT]nix[AT]hotmail[DOT]com
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of version 2 of the GNU General Public License
; as published by the Free Software Foundation.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License along
; with this program; if not, write to the Free Software Foundation, Inc.,
; 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 

bits 16		; 16-bit real mode.
;org  0x0    ; offset to zero.
org 0x500

start:	jmp stage2

;**************************
; include files:
;**************************
%include "stdio.inc"			; standard I/O routines.
%include "gdt.inc"				; GDT load routine.
%include "a20.inc"				; Enable A20 routines.
%include "fat12.inc"			; FAT12 driver.
%include "common.inc"			; common declarations.
%include "multiboot_info.inc"	; MultiBoot Structure.
%include "memory.inc"			; Memory Size functions.
;*********************
; data and variable
;*********************

stage2_msg		db		0xa,0xd,"eqraOS Stage2 Bootloader....",0xa,0xd,0
fail_message	db		0xa,0xd,"KERNEL.EXE is Missing. press any key to reboot...",0


						
boot_info:
istruc multiboot_info
	at multiboot_info.flags,				dd 0
	at multiboot_info.mem_low,				dd 0
	at multiboot_info.mem_high,				dd 0
	at multiboot_info.boot_device,			dd 0
	at multiboot_info.cmd_line,				dd 0
	at multiboot_info.mods_count,			dd 0
	at multiboot_info.mods_addr,			dd 0
	at multiboot_info.sym0,					dd 0
	at multiboot_info.sym1,					dd 0
	at multiboot_info.sym2,					dd 0
	at multiboot_info.mmap_length,			dd 0
	at multiboot_info.mmap_addr,			dd 0
	at multiboot_info.drives_length,		dd 0
	at multiboot_info.drives_addr,			dd 0
	at multiboot_info.config_table,			dd 0
	at multiboot_info.bootloader_name,		dd 0
	at multiboot_info.apm_table,			dd 0
	at multiboot_info.vbe_control_info,		dd 0
	at multiboot_info.vbe_mode_info,		dw 0
	at multiboot_info.vbe_interface_seg,	dw 0
	at multiboot_info.vbe_interface_off,	dw 0
	at multiboot_info.vbe_interface_len,	dw 0
iend


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
	
		mov [boot_info+multiboot_info.boot_device],dl
		
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
		mov si,stage2_msg
		call puts16
	
	;----------------
	; Get Memory Size
	;----------------
		xor eax,eax
		xor ebx,ebx
		call get_memory_size
		
		push eax
		mov eax,64
		mul ebx
		mov ecx,eax
		pop eax
		add eax,ecx
		add eax,1024
		
		mov dword[boot_info+multiboot_info.mem_low],eax
		mov dword[boot_info+multiboot_info.mem_high],0
		
	;----------------
	; Get Memory Map.
	;----------------
		xor eax,eax
		mov ds,ax
		mov di,0x1000
		call get_memory_map
		
	;---------------------
	; Load Root Directory
	;----------------------
		call load_root
		
	;---------------------
	; Load Kernel
	;---------------------0x8d1
		xor ebx,ebx
		mov ebp,KERNEL_RMODE_BASE		; bx:bp buffer to load kernel
		
		mov esi,kernel_name
		call load_file
		
		mov dword[kernel_size],ecx
		cmp ax,0
		je enter_stage3
		
		mov si,fail_message
		call puts16
		
		mov ah,0
		;int 0x16			; wait any key.
		;int 0x19			; warm boot.
		;cli					; cannot go here!
		;hlt
		
		
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

%include "paging.inc"
bad_kernel_msg	db		0xa,0xd,"FATAL: Invalid or corrupt kernel image. Halting system.",0

stage3:
		
	;----------------;
	; Set Registers.
	;----------------;
		
		mov ax,DATA_DESCRIPTOR			; address of data descriptor.
		mov ds,ax
		mov ss,ax
		mov es,ax
		mov esp,0x90000		; stack begin from 0x90000.
		
	;--------------
	; Clear Screen 
	;---------------
		
		call clear_screen
		; debug: b 0x91b

	;---------------
	; Enable Paging
	;---------------
		;call enable_paging
		
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
		
		;b 0x93f
	;------------------------------
	; Insure it is a vaild kernel.
	;------------------------------
	
		mov ebx,[KERNEL_PMODE_BASE+60]
		add ebx,KERNEL_PMODE_BASE			; ebx = _IMAGE_FILE_HEADER
		
		mov esi,ebx
		mov edi,kernel_signature
		
		cmpsw
		
		je execute_kernel
		
		mov ebx,bad_kernel_msg
		call puts32
		
		cli
		hlt
		
	kernel_signature	db	'PE'
	
	;--------------------
	; Execute the kernel.
	;--------------------
	
	execute_kernel:
	
		; because kernel are PE object file we need to parse it, so we can execute kernel_entry routine.
		
		add ebx,24			; ebx = _IMAGE_OPTIONAL_HEADER
		add ebx,16			; ebx point to AddressOfEntryPoint
		
		mov ebp,dword[ebx]	; epb = AddressOfEntryPoint
		
		add ebx,12			; ebx point to ImageBase
		add ebp,dword[ebx]	; epb = kernel_entry
		
		cli
		
		
		; Pass MultiBoot Info to the Kernel
		mov eax,0x2badb002
		mov ebx,0
		mov edx,[kernel_size]
		push dword boot_info
		
		
		call ebp		; Call Kernel
		add esp,4
		
	;-----------------;
	; Hlat the system.
	;-----------------;
		cli			; clear interrupt.		
		hlt			; halt the system.