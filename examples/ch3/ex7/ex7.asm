	;-----------------------------
	; Compute Root Directory Size
	;-----------------------------
	
		xor cx,cx
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
		call read_sectors		