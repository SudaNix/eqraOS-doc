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
		call puts16
		
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