		
	mov ebx,[KERNEL_PMODE_BASE+60]
	add ebx,KERNEL_PMODE_BASE			; ebx = _IMAGE_FILE_HEADER
		
	add ebx,24			; ebx = _IMAGE_OPTIONAL_HEADER
		
	add ebx,16			; ebx point to AddressOfEntryPoint
		
	mov ebp,dword[ebx]	; epb = AddressOfEntryPoint
		
	add ebx,12			; ebx point to ImageBase
		
	add ebp,dword[ebx]	; epb = kernel_entry
		
	cli
		
	call ebp
		