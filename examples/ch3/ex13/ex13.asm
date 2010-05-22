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
		call puts16
		
		pop cx
		pop bx
		pop ax
		
		add ax,1						; next sector
		add bx,word[bytes_per_sector]	; point to next empty block in buffer.
		
	
		loop begin		; cx time
	
		ret