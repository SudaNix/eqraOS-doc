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
	
		mov ax,word[di+26]		; 27 byte in the di entry are cluster number.
		mov word[cluster_number],ax