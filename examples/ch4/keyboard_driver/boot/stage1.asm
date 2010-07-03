; First Stage Bootloader
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
 
 
bits 16				; 16-bit real mode.
org	 0x0			; this number will added to all addresses (relocating).

start:
		jmp main	; jump over data and function to entry point.
		
		
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

hello_msg			db		"eqraOS 0.1 Copyright 2010 Ahmad Essam",0xa,0xd,0
progress_msg		db		".",0
fail_msg			db		"Error: press any key to reboot",0xa,0xd,0

data_region			dw		0
kernel_loader_name	db		"STAGE2  SYS"
cluster_number		dw		0
absolute_sector		db		0
absolute_track		db		0
absolute_head		db		0


; *********************************************
;	print16: prints string using BIOS interrupt
;   input:
;				ds: pointer to data segment.
;				si: point to the string
; *********************************************

print16:
		
		lodsb			; read character from ds:si to al ,and increment si if df=0.
		
		cmp al,0		; check end of string ?
		je end_print16	; yes jump to end.
		
		mov ah,0xe		; print character routine number.
		int 0x10		; call BIOS.
		
		jmp print16		; continue prints until 0 is found.
		
	end_print16:
	
		ret				
		
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
	
	begin:
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
		
		mov si,progress_msg
		call print16
		
		pop cx
		pop bx
		pop ax
		
		add ax,1						; next sector
		add bx,word[bytes_per_sector]	; point to next empty block in buffer.
		
	
		loop begin		; cx time
	
		ret
		
; ***************************************
;		entry point of bootloader.
; ***************************************
		
main:				

		cli				; clear interrupt before set the segment registers.
	;---------------------
	;	intit registers
	;---------------------
		
		; because bootloader are loaded at 0x07c00 we can reference this location with many different combination of segment:offset addressing.
		
		; So we will use either 0x0000:0x7c000 or 0x07c0:0x0000
		; and in this example we use 0x07c0 for segment and 0x0 for offset.
		
		mov ax,0x07c0			
		mov ds,ax
		mov es,ax
		mov gs,ax
		mov fs,ax
		
		; create a stack.
		xor ax,ax
		mov ss,ax
		mov sp,0xffff			; stack start from 0x0:0xffff downward.
		
		
		sti				; restore the interrupt.
		
	;--------------------
	; Display a message.
	;--------------------
	
		mov si,hello_msg
		call print16
	
	;-----------------------------
	; Compute Root Directory Size
	;-----------------------------
	
		xor cx,cx
		xor dx,dx
		
		mov ax,32					; every root entry size are 32 byte.
		mul word[root_directory]	; dx:ax = 32*224 bytes
		div word[bytes_per_sector]	
		xchg ax,cx					; cx = number of sectors to load.
		
	;------------------------------------
	; Get start sector of root directory 
	;------------------------------------
	
		mov al,byte[total_fats]			; there are 2 fats.
		mul word[sectors_per_fat]		; 9*2 sectors
		add ax,word[reserved_sectors]	; ax = start sector of root directory.
		
		mov word[data_region],ax
		add word[data_region],cx	; data_region = start sector of data.
		
		
	;--------------------------------------------------
	; Load Root Dir at 0x07c0:0x0200 above bootloader.
	;--------------------------------------------------
	
		mov bx,0x0200			; es:bs = 0x07c0:0x0200.
		call read_sectors_bios		
		
		
	;-------------------
	; Find stage2.sys
	;-------------------
	
		mov di,0x0200				; di point to first entry in root dir.
		mov cx,word[root_directory]	; loop 224 time.
		
	find_stage2:
	
		mov si,kernel_loader_name	
		push cx
		push di
		mov cx,11					; file name are 11 char long.
		
		rep cmpsb
		pop di
		je find_successfully
		
		mov di,32				; point to next entry.
		pop cx
		
		loop find_stage2
		
		; no found ?
		jmp find_fail
		
	find_successfully:	
	;---------------------
	; Get first Cluster.
	;---------------------
	
		mov ax,word[di+26]		; 26 byte in the di entry are cluster number.
		mov word[cluster_number],ax
		
	
	;---------------------
	; Compute FAT size
	;---------------------
	
		xor cx,cx
		xor ax,ax
		
		mov al,byte[total_fats]			; there are 2 fats.
		mul word[sectors_per_fat]		; 9*2 sectors
		xchg ax,cx
	
	;-------------------------
	; Get start sector of FAT
	;-------------------------
		
		add ax,word[reserved_sectors]
	
	;------------------------------------------------------
	; Load FAT at 0x07c0:0x0200
	; Overwrite Root dir with FAT, no need to Root Dir now.
	;------------------------------------------------------
	
		mov bx,0x0200
		call read_sectors_bios
		
		
		
	;---------------------------------
	; Load all clusters(stage2.sys)
	; At address 0x050:0x0
	;---------------------------------
		
		xor bx,bx
		mov ax,0x0050
		mov es,ax
		
	load_cluster:
		
		mov ax,word[cluster_number]		; ax = cluster number
		call cluster_to_lba				; convert cluster number to LBA addressing.
	
		xor cx,cx
		mov cl,byte[sectors_per_cluster]	; cx = 1 sector
		
		call read_sectors_bios				; load cluster.
		
	read_cluster_fat_entry:
	
		mov ax,word[cluster_number]
		
		; Every FAT entry are 12-bit long( byte and half one).
		; so we must map the cluster number to this entry.
		; to read cluster 0 we need to read fat[0].
		; cluster 1 -> fat[1].
		; cluster 2 -> fat[3],...etc.
		
		mov cx,ax		; cx = cluster number.
		shr cx,1		; divide cx by 2.
		add cx,ax		; cx = ax + (ax/2).
		mov di,cx
		add di,0x0200
		mov dx,word[di]	; read 16-bit form FAT.
		
		
		; Now, because FAT entry are 12-bit long, we should remove 4 bits.
		; if the cluster number are even, we must mask the last four bits.
		; if it odd, we must do four right shift.
		
		test ax,1
		jne odd_cluster
		
	even_cluster:
		
		and dx,0x0fff
		jmp next_cluster
		
	odd_cluster:
		
		shr dx,4
		
	
	next_cluster:
		mov word[cluster_number],dx		; next cluster to load.
		
		cmp dx,0x0ff0					; check end of file, last cluster?
		jb load_cluster					; no, load the next cluster.
		
		
		; yes jmp to end
		jmp end_of_first_stage
		
	find_fail:
	
		mov si,fail_msg
		call print16
		
		mov ah,0x0
		int 0x16		; wait keypress.
		int 0x19		; warm boot.
		
		
	end_of_first_stage:
		
		; jump to stage2 and begin execute.
		push 0x050			; segment number.
		push 0x0			; offset number.
		
		retf			; cs:ip = 0x050:0x0
		
		times	510-($-$$)	db		0		; append zeros.
		
		; finally the boot signature 0xaa55
		db		0x55
		db		0xaa
		