; Listing generated by Microsoft (R) Optimizing Compiler Version 14.00.50727.42 

	TITLE	c:\Documents and Settings\SudaNix\Desktop\eqraOS\eqraOS-doc\examples\ch4\keyboard_driver\core\kernel\mem_info.cpp
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB LIBCMT
INCLUDELIB OLDNAMES

PUBLIC	?boot_info@@3PAUmultiboot_info@@A		; boot_info
PUBLIC	?mem_types@@3PAPADA				; mem_types
PUBLIC	?kernel_size@@3IA				; kernel_size
PUBLIC	?mem_size@@3IA					; mem_size
PUBLIC	?region_ptr@@3PAUmem_region@@A			; region_ptr
_BSS	SEGMENT
?boot_info@@3PAUmultiboot_info@@A DD 01H DUP (?)	; boot_info
?kernel_size@@3IA DD 01H DUP (?)			; kernel_size
?mem_size@@3IA DD 01H DUP (?)				; mem_size
_BSS	ENDS
_DATA	SEGMENT
?mem_types@@3PAPADA DD FLAT:$SG2636			; mem_types
	DD	FLAT:$SG2637
	DD	FLAT:$SG2638
	DD	FLAT:$SG2639
?region_ptr@@3PAUmem_region@@A DD 01000H		; region_ptr
_DATA	ENDS
CONST	SEGMENT
$SG2636	DB	'available', 00H
	ORG $+2
$SG2637	DB	'reserved', 00H
	ORG $+3
$SG2638	DB	'acpi reclaim', 00H
	ORG $+3
$SG2639	DB	'acpi nvs memory', 00H
$SG2659	DB	0aH, 'Physical Memory Map', 0aH, 00H
	ORG $+2
$SG2660	DB	'region      base        size        type', 0aH, 00H
	ORG $+2
$SG2661	DB	'---------------------------------------------------', 0aH
	DB	0aH, 00H
	ORG $+2
$SG2667	DB	'%d            0x%x%x          0x%x%x        %s', 0aH, 00H
$SG2670	DB	0aH, 'Physical memory size: %d KB', 0aH, 00H
	ORG $+6
$SG2673	DB	'Memory statistics: all blocks: %d  ; used blocks: %d  ; '
	DB	'free blocks: %d', 0aH, 00H
CONST	ENDS
PUBLIC	?mem_info_init_region@@YAXXZ			; mem_info_init_region
EXTRN	?pmm_init_region@@YAXII@Z:PROC			; pmm_init_region
; Function compile flags: /Ogtpy
; File c:\documents and settings\sudanix\desktop\eqraos\eqraos-doc\examples\ch4\keyboard_driver\core\kernel\mem_info.cpp
_TEXT	SEGMENT
?mem_info_init_region@@YAXXZ PROC			; mem_info_init_region

; 51   : 
; 52   : 	for (int i=0;i<15;++i) {

	mov	eax, DWORD PTR ?region_ptr@@3PAUmem_region@@A ; region_ptr
	push	ebx
	push	esi
	push	edi
	xor	edi, edi
	xor	esi, esi
	mov	ebx, 1
$LL12@mem_info_i:

; 53   : 		
; 54   : 		// if region is unefined make it reserved.
; 55   : 		if (region_ptr[i].type > 4) 

	cmp	DWORD PTR [esi+eax+16], 4
	jbe	SHORT $LN3@mem_info_i

; 56   : 			region_ptr[i].type = 1;

	mov	DWORD PTR [esi+eax+16], ebx
	mov	eax, DWORD PTR ?region_ptr@@3PAUmem_region@@A ; region_ptr
$LN3@mem_info_i:

; 57   : 			
; 58   : 		// if there is no more region break.
; 59   : 		if (i>0 && region_ptr[i].base_low == 0)

	test	edi, edi
	jle	SHORT $LN2@mem_info_i
	cmp	DWORD PTR [esi+eax], 0
	je	SHORT $LN10@mem_info_i
$LN2@mem_info_i:

; 60   : 			break;
; 61   : 			
; 62   : 		// init region if available
; 63   : 		if (region_ptr[i].type == 1)

	cmp	DWORD PTR [esi+eax+16], ebx
	jne	SHORT $LN5@mem_info_i

; 64   : 			pmm_init_region(region_ptr[i].base_low,region_ptr[i].length_low);

	mov	ecx, DWORD PTR [esi+eax+8]
	mov	edx, DWORD PTR [esi+eax]
	push	ecx
	push	edx
	call	?pmm_init_region@@YAXII@Z		; pmm_init_region
	mov	eax, DWORD PTR ?region_ptr@@3PAUmem_region@@A ; region_ptr
	add	esp, 8
$LN5@mem_info_i:
	add	esi, 24					; 00000018H
	add	edi, ebx
	cmp	esi, 360				; 00000168H
	jl	SHORT $LL12@mem_info_i
$LN10@mem_info_i:
	pop	edi
	pop	esi
	pop	ebx

; 65   : 	}
; 66   : }

	ret	0
?mem_info_init_region@@YAXXZ ENDP			; mem_info_init_region
_TEXT	ENDS
PUBLIC	?mem_info_dump_region@@YAXXZ			; mem_info_dump_region
EXTRN	?kprintf@@YAHPBDZZ:PROC				; kprintf
EXTRN	?kputs@@YAXPAD@Z:PROC				; kputs
EXTRN	?kset_color@@YAII@Z:PROC			; kset_color
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
?mem_info_dump_region@@YAXXZ PROC			; mem_info_dump_region

; 68   : void mem_info_dump_region() {

	push	esi
	push	edi

; 69   : 	kset_color(0x19);

	push	25					; 00000019H
	call	?kset_color@@YAII@Z			; kset_color

; 70   : 	kputs("\nPhysical Memory Map\n");

	push	OFFSET $SG2659
	call	?kputs@@YAXPAD@Z			; kputs

; 71   : 	kputs("region      base        size        type\n");

	push	OFFSET $SG2660
	call	?kputs@@YAXPAD@Z			; kputs

; 72   : 	kputs("---------------------------------------------------\n\n");

	push	OFFSET $SG2661
	call	?kputs@@YAXPAD@Z			; kputs
	add	esp, 16					; 00000010H

; 73   : 	
; 74   : 	for (int i=0;i<15;++i) {

	xor	edi, edi
	xor	esi, esi
	npad	2
$LL4@mem_info_d:

; 75   : 	
; 76   : 		// if there is no more region break.
; 77   : 		if (i>0 && region_ptr[i].base_low == 0)

	test	edi, edi
	mov	eax, DWORD PTR ?region_ptr@@3PAUmem_region@@A ; region_ptr
	jle	SHORT $LN1@mem_info_d
	cmp	DWORD PTR [esi+eax], 0
	je	SHORT $LN8@mem_info_d
$LN1@mem_info_d:

; 78   : 			break;
; 79   : 			
; 80   : 		// dump entry.
; 81   : 		kprintf("%d            0x%x%x          0x%x%x        %s\n",
; 82   : 			i,
; 83   : 			region_ptr[i].base_high,region_ptr[i].base_low,
; 84   : 			region_ptr[i].length_high,region_ptr[i].length_low,
; 85   : 			mem_types[region_ptr[i].type-1]
; 86   : 			);

	mov	ecx, DWORD PTR [esi+eax+16]
	mov	edx, DWORD PTR ?mem_types@@3PAPADA[ecx*4-4]
	mov	ecx, DWORD PTR [esi+eax+8]
	push	edx
	mov	edx, DWORD PTR [esi+eax+12]
	push	ecx
	mov	ecx, DWORD PTR [esi+eax]
	push	edx
	mov	edx, DWORD PTR [esi+eax+4]
	push	ecx
	push	edx
	push	edi
	push	OFFSET $SG2667
	call	?kprintf@@YAHPBDZZ			; kprintf
	add	esi, 24					; 00000018H
	add	esp, 28					; 0000001cH
	add	edi, 1
	cmp	esi, 360				; 00000168H
	jl	SHORT $LL4@mem_info_d
$LN8@mem_info_d:

; 87   : 	}
; 88   : 
; 89   : 	kset_color(0x17);

	push	23					; 00000017H
	call	?kset_color@@YAII@Z			; kset_color
	add	esp, 4
	pop	edi
	pop	esi

; 90   : }

	ret	0
?mem_info_dump_region@@YAXXZ ENDP			; mem_info_dump_region
_TEXT	ENDS
PUBLIC	?mem_info_dump_size@@YAXXZ			; mem_info_dump_size
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
?mem_info_dump_size@@YAXXZ PROC				; mem_info_dump_size

; 93   : 	kprintf("\nPhysical memory size: %d KB\n",mem_size);

	mov	eax, DWORD PTR ?mem_size@@3IA		; mem_size
	push	eax
	push	OFFSET $SG2670
	call	?kprintf@@YAHPBDZZ			; kprintf
	add	esp, 8

; 94   : 	//kprintf("memory low: %d    ;  memory high: %d \n\n",boot_info->m_mem_low,boot_info->m_mem_high);
; 95   : }

	ret	0
?mem_info_dump_size@@YAXXZ ENDP				; mem_info_dump_size
_TEXT	ENDS
PUBLIC	?mem_info_blocks_stat@@YAXXZ			; mem_info_blocks_stat
EXTRN	?pmm_get_block_count@@YAIXZ:PROC		; pmm_get_block_count
EXTRN	?pmm_get_used_block_count@@YAIXZ:PROC		; pmm_get_used_block_count
EXTRN	?pmm_get_free_block_count@@YAIXZ:PROC		; pmm_get_free_block_count
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
?mem_info_blocks_stat@@YAXXZ PROC			; mem_info_blocks_stat

; 98   : 	kset_color(GRAY_ON_BLUE);

	push	23					; 00000017H
	call	?kset_color@@YAII@Z			; kset_color
	add	esp, 4

; 99   : 	
; 100  : 	kprintf("Memory statistics: all blocks: %d  ; used blocks: %d  ; free blocks: %d\n",
; 101  : 		pmm_get_block_count(),
; 102  : 		pmm_get_used_block_count(),
; 103  : 		pmm_get_free_block_count()
; 104  : 		);

	call	?pmm_get_free_block_count@@YAIXZ	; pmm_get_free_block_count
	push	eax
	call	?pmm_get_used_block_count@@YAIXZ	; pmm_get_used_block_count
	push	eax
	call	?pmm_get_block_count@@YAIXZ		; pmm_get_block_count
	push	eax
	push	OFFSET $SG2673
	call	?kprintf@@YAHPBDZZ			; kprintf
	add	esp, 16					; 00000010H

; 105  : }

	ret	0
?mem_info_blocks_stat@@YAXXZ ENDP			; mem_info_blocks_stat
_TEXT	ENDS
PUBLIC	?mem_info_init@@YAXPAUmultiboot_info@@@Z	; mem_info_init
EXTRN	?pmm_deinit_region@@YAXII@Z:PROC		; pmm_deinit_region
EXTRN	?pmm_init@@YAXII@Z:PROC				; pmm_init
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_info$ = 8						; size = 4
?mem_info_init@@YAXPAUmultiboot_info@@@Z PROC		; mem_info_init

; 28   : 
; 29   : 	// Multiboot informtion.
; 30   : 	boot_info = info;

	mov	eax, DWORD PTR _info$[esp-4]
	mov	DWORD PTR ?boot_info@@3PAUmultiboot_info@@A, eax ; boot_info

; 31   : 
; 32   : 	// kernel size
; 33   : 	_asm mov word ptr [kernel_size],dx

	mov	WORD PTR ?kernel_size@@3IA, dx

; 34   : 	
; 35   : 	// Memory size in KB
; 36   : 	// mem_size = 1024 + boot_info->m_mem_low + boot_info->m_mem_high * 64; computed from bootloader .
; 37   : 	mem_size = boot_info->m_mem_size;
; 38   : 	
; 39   : 	// Init pmm, put memory map bit above the kernel.
; 40   : 	pmm_init(mem_size,KERNEL_BASE + kernel_size*512);

	mov	edx, DWORD PTR ?kernel_size@@3IA	; kernel_size
	mov	ecx, DWORD PTR ?boot_info@@3PAUmultiboot_info@@A ; boot_info
	mov	eax, DWORD PTR [ecx+4]
	add	edx, 2048				; 00000800H
	shl	edx, 9
	push	edx
	push	eax
	mov	DWORD PTR ?mem_size@@3IA, eax		; mem_size
	call	?pmm_init@@YAXII@Z			; pmm_init

; 41   : 
; 42   : 	// Unset all non-reserved regions.
; 43   : 	mem_info_init_region();

	call	?mem_info_init_region@@YAXXZ		; mem_info_init_region

; 44   : 
; 45   : 	// kernel region is reserved (set).
; 46   : 	pmm_deinit_region(KERNEL_BASE,kernel_size*512);

	mov	eax, DWORD PTR ?kernel_size@@3IA	; kernel_size
	shl	eax, 9
	push	eax
	push	1048576					; 00100000H
	call	?pmm_deinit_region@@YAXII@Z		; pmm_deinit_region
	add	esp, 16					; 00000010H

; 47   : }

	ret	0
?mem_info_init@@YAXPAUmultiboot_info@@@Z ENDP		; mem_info_init
_TEXT	ENDS
END