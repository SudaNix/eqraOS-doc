
read_sectors:

	reset_floppy:

		mov ah,0x0		; reset floppy routine number.
		mov dl,0x0		; drive number
		
		int 0x13		; call BIOS
		
		jc reset_floppy		; try again if error occur.
	
		; init buffer.
		mov ax,0x1000
		mov es,ax
		xor bx,bx
	
	read:
	
		mov ah,0x2		; routine number.
		mov al,1		; how many sectors?
		mov ch,1		; cylinder or track number.
		mov cl,2		; sector number "fisrt sector is 1 not 0",now we read the second sector.
		mov dh,0		; head number "starting with 0".
		mov dl,0		; drive number ,floppy drive always zero.
		
		int 0x13		; call BIOS.
		jc read			; if error, try again.
		
		jmp 0x1000:0x0	; jump to execute the second sector.
	