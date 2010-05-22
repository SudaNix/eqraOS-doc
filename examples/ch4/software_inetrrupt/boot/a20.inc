; Enable A20 gate routines.
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

%ifndef A20_INC
%define A20_INC

bits	16		; still in real mode.

;*************************************************
; enable_a20_port_0x92:
;		Enable A20 with System Control port 0x92
;*************************************************

enable_a20_port_0x92:

		push ax			; save register.
		
		mov al,2		; set bit 2 to enable A20
		out 0x92,al
		
		pop ax			; restore register.
		ret


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

		
;****************************************************
; enable_a20_keyboard_controller:
;		Enable A20 with command 0xdd
;****************************************************

enable_a20_keyboard_controller:

		;cli
		push ax			; save register.
		
		mov al,0xdd		; Enable A20 Keyboard Controller Command.
		out 0x64,al
		
		pop ax			; restore register.
		ret

		
;****************************************************
; enable_a20_keyboard_controller_output_port:
;		Enable A20 with write to keyboard output port.
;****************************************************

enable_a20_keyboard_controller_output_port:

		cli
		pusha			; save all registers
		
		call wait_input		; wait last operation to be finished.
		
		;------------------
		; Disable Keyboard
		;------------------
		mov al,0xad		; disable keyboard command.
		out 0x64,al
		call wait_input
		
		;-------------------------------
		; send read output port command
		;--------------------------------
		mov al,0xd0		; read output port command
		out 0x64,al
		call wait_output		; wait output to come.
		; we don't need to wait_input bescause when output came we know that operation are executed.
		
		;----------------------
		; read input buffer
		;----------------------
		in al,0x60
		push eax		; save data.
		call wait_input
		
		;--------------------------------
		; send write output port command.
		;--------------------------------
		mov al,0xd1		; write output port command.
		out 0x64,al
		call wait_input
		
		;------------
		; enable a20.
		;------------
		pop eax
		or al,2			; set bit 2.
		out 0x60,al
		call wait_input
		
		;-----------------
		; Enable Keyboard.
		;-----------------
		mov al,0xae		; Enable Keyboard command.
		out 0x64,al
		call wait_input
		
		
		popa			; restore registers
		sti
		
		ret
		
		
;*******************************************
; wait_output: wait output buffer to be full.
;*******************************************

wait_output:

		in al,0x64		; read status
		test al,0x1		; is output buffer is empty?
		je wait_output	; yes, hang.
		
		ret				; no,there is a result.
		
		
;******************************************
; wait_input: wait input buffer to be empty.
;				command executed already.
;******************************************

wait_input:
	
		in al,0x64		; read status
		test al,0x2		; is input buffer is full?
		jne wait_input	; yes, hang.
		
		ret				; no,command executed.
		
		
%endif	;A20_INC
