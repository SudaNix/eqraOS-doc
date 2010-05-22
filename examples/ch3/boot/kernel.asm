; Hello World Kernel.
; This is a pure binary kernel to test our work.
; it will be replaced with C kernel (ELF or PE object code).
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


org		0x100000			; kernel will load at 1 MB.

bits	32					; PMode.

jmp kernel_entry

%include "stdio.inc"


	
kernel_message	db		0xa,0xa,0xa,"               eqraOS v0.1 Copyright (C) 2010 Ahmad Essam"
				db		0xa,0xa,    "          University of Khartoum - Faculty of Mathematical Sceinces.",0


logo_message	db		0xa,0xa,0xa,"                ___ ___ ________ _  / __ \ / __/"
				db		0xa,        "               / -_) _ `/ __/ _ `/ / /_/ /_\ \  "
				db		0xa,        "               \__/\_, /_/  \_,_/  \____//___/  "
				db		0xa,        "                    /_/                         ",0
;*********************
; Entry point.
;*********************

kernel_entry:
		
	;---------------
	; Set Registers
	;---------------
		
		mov ax,0x10			; data selector.
		mov	ds,ax
		mov es,ax
		mov ss,ax
		mov esp,0x90000		; set stack.
		
	;---------------------------------
	; Clear Screen and print message.
	;---------------------------------
	
		call clear_screen
		
		mov ebx,kernel_message
		call puts32
		
		mov ebx,logo_message
		call puts32
		
	;-----------------
	; Halt the system.
	;-----------------
	
		cli
		hlt
		
