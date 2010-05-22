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
%include "stdio.inc"		; standard I/O routines.
%include "gdt.inc"			; GDT load routine.
%include "a20.inc"			; Enable A20 routines.
%include "fat12.inc"		; FAT12 driver.
%include "common.inc"		; common declarations.

;*********************
; data and variable
;*********************

stage2_msg		db		0xa,0xd,"eqraOS Stage2 Bootloader....",0xa,0xd,0
fail_message	db		0xa,0xd,"KERNEL.EXE is Missing. press any key to reboot...",0
bad_kernel_msg	db		0xa,0xd,"FATAL: Invalid or corrupt kernel image. Halting system.",0

						

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
		mov si,stage2_msg
		call puts16
		
	;---------------------
	; Load Root Directory
	;----------------------
		call load_root
		
	;---------------------
	; Load Kernel
	;---------------------0x8d1
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
		
	;--------------
	; Clear Screen 
	;---------------
		
		call clear_screen
		; debug: b 0x91b

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
		
		call ebp
		
		
	;-----------------;
	; Hlat the system.
	;-----------------;
		cli			; clear interrupt.		
		hlt			; halt the system.
		

		
;-- header information format for PE files -------------------

;typedef struct _IMAGE_DOS_HEADER {  // DOS .EXE header
;    USHORT e_magic;         // Magic number (Should be MZ
;    USHORT e_cblp;          // Bytes on last page of file
;    USHORT e_cp;            // Pages in file
;    USHORT e_crlc;          // Relocations
;    USHORT e_cparhdr;       // Size of header in paragraphs
;    USHORT e_minalloc;      // Minimum extra paragraphs needed
;    USHORT e_maxalloc;      // Maximum extra paragraphs needed
;    USHORT e_ss;            // Initial (relative) SS value
;    USHORT e_sp;            // Initial SP value
;    USHORT e_csum;          // Checksum
;    USHORT e_ip;            // Initial IP value
;    USHORT e_cs;            // Initial (relative) CS value
;    USHORT e_lfarlc;        // File address of relocation table
;    USHORT e_ovno;          // Overlay number
;    USHORT e_res[4];        // Reserved words
;    USHORT e_oemid;         // OEM identifier (for e_oeminfo)
;    USHORT e_oeminfo;       // OEM information; e_oemid specific
;    USHORT e_res2[10];      // Reserved words
;    LONG   e_lfanew;        // File address of new exe header
;  } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

;<<------ Real mode stub program -------->>

;<<------ Here is the file signiture, such as PE00 for NT --->>

;typedef struct _IMAGE_FILE_HEADER {
;    USHORT  Machine;
;    USHORT  NumberOfSections;
;    ULONG   TimeDateStamp;
;    ULONG   PointerToSymbolTable;
;    ULONG   NumberOfSymbols;
;    USHORT  SizeOfOptionalHeader;
;    USHORT  Characteristics;
;} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

;struct _IMAGE_OPTIONAL_HEADER {
;    //
;    // Standard fields.
;    //
;    USHORT  Magic;
;    UCHAR   MajorLinkerVersion;
;    UCHAR   MinorLinkerVersion;
;    ULONG   SizeOfCode;
;    ULONG   SizeOfInitializedData;
;    ULONG   SizeOfUninitializedData;
;    ULONG   AddressOfEntryPoint;			<< IMPORTANT!
;    ULONG   BaseOfCode;
;    ULONG   BaseOfData;
;    //
;    // NT additional fields.
;    //
;    ULONG   ImageBase;
;    ULONG   SectionAlignment;
;    ULONG   FileAlignment;
;    USHORT  MajorOperatingSystemVersion;
;    USHORT  MinorOperatingSystemVersion;
;    USHORT  MajorImageVersion;
;    USHORT  MinorImageVersion;
;    USHORT  MajorSubsystemVersion;
;    USHORT  MinorSubsystemVersion;
;    ULONG   Reserved1;
;    ULONG   SizeOfImage;
;    ULONG   SizeOfHeaders;
;    ULONG   CheckSum;
;    USHORT  Subsystem;
;    USHORT  DllCharacteristics;
;    ULONG   SizeOfStackReserve;
;    ULONG   SizeOfStackCommit;
;    ULONG   SizeOfHeapReserve;
;    ULONG   SizeOfHeapCommit;
;    ULONG   LoaderFlags;
;    ULONG   NumberOfRvaAndSizes;
;    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
;} IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;


