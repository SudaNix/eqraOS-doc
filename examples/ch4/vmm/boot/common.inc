; common.inc: common constant and variable.
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

%ifndef COMMAN_INC
%define COMMAN_INC

; Place to load kernel in real mode.
%define KERNEL_RMODE_BASE 0x3000

; Place to load kernel in protected mode.
%define KERNEL_PMODE_BASE 0x100000

;%define KERNEL_PMODE_BASE  0xC0000000

; Kernel image name.
kernel_name	db	"KERNEL  EXE"

; Size of kernel in bytes.
kernel_size	db	0


%endif	; COMMAN_INC