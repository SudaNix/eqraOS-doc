reset_floppy:

		mov ah,0x0		; reset floppy routine number.
		mov dl,0x0		; drive number
		
		int 0x13		; call BIOS
		
		jc reset_floppy		; try again if error occur.