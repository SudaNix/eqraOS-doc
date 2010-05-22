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