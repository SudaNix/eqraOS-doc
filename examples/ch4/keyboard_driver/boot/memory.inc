%ifndef MEMORY_INC
%define MEMORY_INC

bits 16

; Memory Map Entry Structure
struc memory_map_entry
	.base_addr	resq	1
	.length		resq	1
	.type		resd	1
	.acpi_null	resd	1
endstruc


; -------------------------------------------------
; get_memory_map:
;	Input:
;		EAX = 0x0000E820
;		EBX = continuation value or 0 to start at beginning of map
;		ECX = size of buffer for result (Must be >= 20 bytes)
;		EDX = 0x534D4150h ('SMAP')
;		ES:DI = Buffer for result
;
;	Return:
;		CF = clear if successful
;		EAX = 0x534D4150h ('SMAP')
;		EBX = offset of next entry to copy from or 0 if done
;		ECX = actual length returned in bytes
;		ES:DI = buffer filled
;		If error, AH containes error code 
; --------------------------------------------------
get_memory_map:
		xor ebx,ebx
		xor bp,bp
		mov edx,0x0534D4150
		mov eax,0xe820
		mov	[es:di + 20], dword 1
		mov ecx,24
		int 0x15			; BIOS get memory map.
		
		jc short .error
		mov edx,0x0534D4150
		cmp eax,edx
		jne short .error
		
		test ebx,ebx
		je short .error
		
		jmp short .start
		
	.next_entry:
		mov eax,0xe820
		mov	[es:di + 20], dword 1
		mov ecx,24
		int 0x15			; BIOS get memory map.
		jc		short .done
		mov edx,0x534D4150
		
	.start:
		jcxz .skip_entry
		cmp		cl, 20	
		jbe		short .notext
		test	byte [es:di + 20], 1	
		je		short .skip_entry
	.notext:
		mov		ecx, [es:di + 8]	
		test	ecx, ecx				
		jne		short .good_entry
		mov		ecx, [es:di + 12]		
		jecxz	.skip_entry				

	.good_entry:
		inc bp
		add di,24
	
	.skip_entry:
		test ebx,ebx
		jne .next_entry
		jmp .done
		
	.error:
		stc				; set carry flag
		ret
		
	.done:
		ret
		

; --------------------------
; get memory size_32bit:
;	return:
;		ax=KB between 1MB and 16MB
;		bx=number of 64K blocks above 16MB
;		bx=0 and ax= -1 on error
; --------------------------		
get_memory_size_32bit:

		push ecx
		push edx
		xor ecx,ecx
		xor edx,edx
		
		mov ax,0xe881		; BIOS get memory size
		int 0x15
		
		jc .error
		cmp ah,0x86
		je .error
		cmp ah,0x80
		je .error
		
		jcxz .use_eax
		
		mov ax,cx
		mov bx,dx
		
	.use_eax:
		pop edx
		pop ecx
		ret
		
	.error:
		mov ax,-1
		mov bx,0
		pop edx
		pop ecx
		ret
		
	
; --------------------------
; get memory size:
; get a total memory size.except the first MB. 
;	return:
;		ax=KB between 1MB and 16MB
;		bx=number of 64K blocks above 16MB
;		bx=0 and ax= -1 on error
; --------------------------		
get_memory_size:

		push ecx
		push edx
		xor ecx,ecx
		xor edx,edx
		
		mov ax,0xe801		; BIOS get memory size
		int 0x15
		
		jc .error
		cmp ah,0x86
		je .error
		cmp ah,0x80
		je .error
		
		jcxz .use_eax
		
		mov ax,cx
		mov bx,dx
		
	.use_eax:
		pop edx
		pop ecx
		ret
		
	.error:
		mov ax,-1
		mov bx,0
		pop edx
		pop ecx
		ret
		

;---------------------------------
; get_conventional_memory_size
;	ret ax=KB size from address 0
;--------------------------------
get_conventional_memory_size:
	int 0x12
	ret
	
;------------------------------------------
; get_extended_memory_size
;	ret ax=KB size above 1MB; ax= -1 on error
;--------------------------------------------	
get_extended_memory_size:
		mov ax,0x88
		int 0x15
	
		jc .error
		test ax,ax
		je .error
		cmp ah,0x86
		je .error
		cmp ah,0x80
		je .error
	
		ret
	
	.error:
		mov ax,-1
		ret

		
%endif ; MEMORY_INC