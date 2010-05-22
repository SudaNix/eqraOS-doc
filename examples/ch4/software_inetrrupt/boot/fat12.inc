; Fat12 driver.
; driver now just support read files.
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

%ifndef FAT12_INC
%define FAT12_INC

bits 16				; 16-bit real mode.

%include "floppy16.inc"

; Addresses to load root directory and fat table.

%define ROOT_OFFSET		0x2e00
%define ROOT_SEGMENT	0x02e0

%define FAT_SEGMENT		0x2c0

;*****************************************
; load_root:
;		load root directory at 0x2e0:0x0
;*****************************************

load_root:
		
		pusha			; Save Registers.
		push	es
		
		
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
		
		
	;------------------------------------
	; Load Root Dir at ROOT_SEGMENT:0x0 
	;------------------------------------
		
		push word ROOT_SEGMENT
		pop es
		
		mov bx,0x0			; es:bs = 0x2e0:0x0.
		
		call read_sectors_bios	
		
		pop es				; Restore es.
		popa				; Restore Registers
		
		ret
		

;**********************************
; load_fat:
;		load fat table.
;**********************************

load_fat:
		
		pusha				; Save Registers.
		push	es
		
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
	; Load FAT at FAT_SEGMENT:0x0
	;------------------------------------------------------
	
		push word FAT_SEGMENT
		pop es
		
		mov bx,0x0			; es:bs = 0x2c0:0x0.
		
		call read_sectors_bios
		
		pop es				; Restore es.
		popa				; Restore Registers
		
		ret

;********************************************
; find_file:
;		find file in root directory.
;
; input:
;		ds:si	name of the kernel.
;
; output:
;		ax		return value: file index. -1 on error.
;********************************************

find_file:

		push cx
		push dx
		push bx
		
		mov bx,si
		
	;-------------------
	; Find kerenl
	;-------------------
		
		mov di,ROOT_OFFSET				; di point to first entry in root dir.
		mov cx,word[root_directory]		; loop 224 time.
		cld
		
	find_kernel:
	
		mov si,bx	
		push cx
		push di
		mov cx,11					; file name are 11 char long.
		
		rep cmpsb
		pop di
		je find_successfully
		
		add di,32				; point to next entry.
		pop cx
		
		loop find_kernel
		
		; no found ?
		pop bx
		pop dx
		pop cx
		mov ax,-1			; error
		ret
		
	find_successfully:	

		pop ax
		pop bx
		pop dx
		pop cx
		
		ret

		
		
;********************************************
; load_file:
;		load all clusters of the file(kernel).
;
; input:
;		ds:si	name of the kernel.
;		bx:bp	buffer to read kernel.
;
; output:
;		cx		number of kernel sectors.
;		ax		return value: -1 on error.		
;********************************************

load_file:

		xor ecx,ecx					; count kernel sectors.
		push ecx					; save it.
		push bx
		push bp
		
	;--------------------
	; Find file.
	;--------------------
	
		call find_file
		cmp ax,-1
		jne get_cluster
		
		pop bp
		pop bx
		pop ecx
		mov ax,-1				; error
		ret
	
	;---------------------
	; Get first Cluster.
	;---------------------
	
	get_cluster:
	
		sub edi,ROOT_OFFSET
		sub eax,ROOT_OFFSET
		
		push word ROOT_SEGMENT
		pop es
		
		mov dx,word[es:di+26]		; 26 byte in the di entry are cluster number.
		mov word[cluster_number],dx
		
		pop bx
		pop es
		
		push bx
		push es
		
		call load_fat
		
		
	;---------------------
	; Load all clusters
	;---------------------
		
	load_cluster:
		
		mov ax,word[cluster_number]		; ax = cluster number
		pop es
		pop bx
		
		call cluster_to_lba				; convert cluster number to LBA addressing.
	
		xor cx,cx
		mov cl,byte[sectors_per_cluster]	; cx = 1 sector
		
		call read_sectors_bios				; load cluster.
		
		pop ecx
		inc ecx
		push ecx
		push bx
		push es
		
		mov ax,FAT_SEGMENT
		mov es,ax
		xor bx,bx
		
		
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
		
		mov bx,0x0
		add bx,cx
		
		mov dx,word[es:bx]	; read 16-bit form FAT.
		
		
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
		
		

		pop es
		pop bx
		pop ecx
		xor ax,ax			; success.
		ret
		

%endif	; FAT12_INC