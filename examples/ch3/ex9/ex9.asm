	;---------------------
	; Compute FAT size
	;---------------------
	
		xor cx,cx
		xor ax,ax
		xor dx,dx
		
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
		call read_sectors