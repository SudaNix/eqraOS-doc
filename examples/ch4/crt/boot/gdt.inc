; gdt.inc: Create Global Descriptor Table
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


%ifndef GTD_INC
%define GDT_INC

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


;**************************************
; gdt_ptr: data structure used by gdtr
;**************************************

gdt_ptr:
		
		dw end_of_gdt - begin_of_gdt - 1		; size -1
		dd begin_of_gdt							; base of gdt
		


%define NULL_DESCRIPTOR	0x0
%define CODE_DESCRIPTOR 0x8
%define DATA_DESCRIPTOR 0x10

%endif  ; GDT_INC