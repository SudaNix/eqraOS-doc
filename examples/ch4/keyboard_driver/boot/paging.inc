; paging.inc: Basic paging for bootloader.
; A modified code of brokenthorn.com
 
%ifndef PAGING_INC
%define PAGING_INC

bits 32

%define PAGE_DIR			0x9C000
%define	PAGE_TABLE_0		0x9D000
%define	PAGE_TABLE_768		0x9E000
%define	PAGE_TABLE_ENTRIES	1024
%define PAGE_ATTRIB_PWK		3

enable_paging:
		pusha
	
		; idenitity map first page table (4MB).
	
		mov eax, PAGE_TABLE_0
		mov ebx, 0x0 | PAGE_ATTRIB_PWK
		mov ecx, PAGE_TABLE_ENTRIES
	
	.loop:
		mov dword[eax],ebx		; write page entry
		mov eax,4				; next page
		add ebx,4096
		loop .loop
		
		; setup the entries in the page directory table.
		
		mov eax, PAGE_TABLE_0 | PAGE_ATTRIB_PWK
		mov dword[PAGE_DIR], eax
		
		mov eax, PAGE_TABLE_768 | PAGE_ATTRIB_PWK
		mov dword[PAGE_DIR+(768*4)], eax
		
		
		; install page directory table
		mov eax, PAGE_DIR
		mov cr3, eax
		
		; enable paging
		mov eax,cr0
		or eax,0x80000000
		mov cr0,eax
		
		;	map the 768th table to physical addr 1MB
		;	the 768th table starts the 3gb virtual address
		
		mov eax, PAGE_TABLE_768
		mov ebx, 0x100000 | PAGE_ATTRIB_PWK
		mov ecx, PAGE_TABLE_ENTRIES
		
	.loop2:
		mov dword[eax],ebx		; write page entry
		mov eax,4				; next page
		add ebx,4096
		loop .loop2
		
		
		popa
		ret
	
%endif ; PAGING_INC
