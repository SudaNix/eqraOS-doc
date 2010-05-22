; 16-bit (BIOS interrupt) Floppy disk driver.
; driver now just support read sectors.
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

%ifndef FLOPPY16_INC
%define FLOPPY16_INC

bits 16				; 16-bit real mode.

;******************************************
; OEM Id and BIOS Parameter Block (BPB) 
;******************************************

; Must begin at byte 3(4th byte), if not we should add nop instruction.

OEM_ID               db        "eqraOS  "    ; Name of your OS, Must be 8 byte! no more no less.

bytes_per_sector     dw        0x200		; 512 byte per sector.
sectors_per_cluster  db        0x1          ; 1 sector per cluster.
reserved_sectors     dw        0x1          ; boot sector is reserved.
total_fats           db        0x2          ; two fats.
root_directory       dw        0xe0         ; root dir has 224 entries.
total_sectors        dw        0xb40        ; 2880 sectors in the volume.
media_descriptor     db        0xf8         ; 1.44 floppy disk.
sectors_per_fat      dw        0x9          ; 9 sector per fat.
sectors_per_track    dw        0x12         ; 18 sector per track.
number_of_heads      dw        0x2          ; 2 heads per platter.
hidden_sectors       dd        0x0          ; no hidden sector.
total_sectors_large  dd        0x0

; Extended BPB.

drive_number         db        0x0			; 0 for 1.44MB floppy disk drive.
flags                db        0x0
signature            db        0x29         ; must be 0x28 or 0x29.
volume_id            dd        0x0          ; serial number written when foramt the disk.
volume_label         db        "EOS FLOPPY " ; 11 byte.
system_id            db        "fat12   "    ;  8 byte.


; ****************
;	data
; ****************


data_region			dw		0
cluster_number		dw		0
absolute_sector		db		0
absolute_track		db		0
absolute_head		db		0


; **************************************************
;	cluster_to_lba: convert cluster number to LBA
;   input:
;				ax: Cluster number.
;	output:
;				ax: lba number.
; **************************************************
cluster_to_lba:
		
		; lba = (cluster - 2)* sectors_per_cluster
		; the first cluster is always 2.
		
		sub ax,2
		
		xor cx,cx
		mov cl, byte[sectors_per_cluster]
		mul cx
		
		add ax,word[data_region]		; cluster start from data area.
		ret

; *********************************************
;	lba_to_chs: Convert LBA to CHS.
;   input:
;				ax: LBA.
;	output:
;				absolute_sector
;				absolute_track
;				absolute_head
; *********************************************
lba_to_chs:

		; absolute_sector =  (lba % sectors_per_track) + 1
		; absolute_track  =  (lba / sectors_per_track) / number_of_heads
		; absolute_head   =  (lba / sectors_per_track) % number_of_heads

		xor dx,dx
		div word[sectors_per_track]
		inc dl
		mov byte[absolute_sector],dl
		
		xor dx,dx
		div word[number_of_heads]
		mov byte[absolute_track],al
		mov byte[absolute_head],dl
		
		ret
		
; *********************************************
;	read_sectors_bios: load sector from floppy disk
;   input:
;				es:bx : Buffer to load sector.
;				ax: 	first sector number ,LBA.
;				cx:		number of sectors.
; *********************************************
read_sectors_bios:
	
	begin_read:
		mov di,5		; try 5 times to load any sector.
		
	load_sector:
		
		push ax
		push bx
		push cx

		call lba_to_chs
		
		mov ah,0x2						; load sector routine number.
		mov al,0x1						; 1 sector to read.
		mov ch,byte[absolute_track]		; absolute track number.
		mov cl,byte[absolute_sector]	; absolute sector number.
		mov dh,byte[absolute_head]		; absolute head number.
		mov dl,byte[drive_number]		; floppy drive number.
		
		int 0x13						; call BIOS.
		
		jnc continue	; if no error jmp.
		
		; reset the floppy and try read again.
		
		mov ah,0x0					; reset routine number.
		mov dl,0x0					; floppy drive number.
		int 0x13					; call BIOS.
		
		pop cx
		pop bx
		pop ax
		
		dec di
		jne load_sector
		
		; error.
		int 0x18
		
	continue:
		
		;mov si,progress_msg
		;call print16
		
		pop cx
		pop bx
		pop ax
		
		add ax,1						; next sector
		add bx,word[bytes_per_sector]	; point to next empty block in buffer.
		
	
		loop begin_read		; cx time
	
		ret
		

%endif	; FLOPPY16_INC