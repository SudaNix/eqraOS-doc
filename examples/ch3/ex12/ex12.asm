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