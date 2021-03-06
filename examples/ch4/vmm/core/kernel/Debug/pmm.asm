; Listing generated by Microsoft (R) Optimizing Compiler Version 14.00.50727.42 

	TITLE	c:\Documents and Settings\SudaNix\Desktop\eqraOS\eqraOS-doc\examples\ch4\vmm\core\kernel\pmm.cpp
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB LIBCMT
INCLUDELIB OLDNAMES

_BSS	SEGMENT
__pmm_mem_size DD 01H DUP (?)
__pmm_max_blocks DD 01H DUP (?)
__pmm_used_blocks DD 01H DUP (?)
__pmm_mmap DD	01H DUP (?)
_BSS	ENDS
PUBLIC	?mmap_set@@YAXH@Z				; mmap_set
; Function compile flags: /Ogtpy
; File c:\documents and settings\sudanix\desktop\eqraos\eqraos-doc\examples\ch4\vmm\core\kernel\pmm.cpp
;	COMDAT ?mmap_set@@YAXH@Z
_TEXT	SEGMENT
_bit$ = 8						; size = 4
?mmap_set@@YAXH@Z PROC					; mmap_set, COMDAT

; 16   : 	_pmm_mmap[bit/32] = _pmm_mmap[bit/32] | (1 << (bit%32)) ;

	mov	ecx, DWORD PTR _bit$[esp-4]
	mov	eax, ecx
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	mov	edx, DWORD PTR __pmm_mmap
	sar	eax, 5
	and	ecx, -2147483617			; 8000001fH
	lea	eax, DWORD PTR [edx+eax*4]
	jns	SHORT $LN3@mmap_set
	dec	ecx
	or	ecx, -32				; ffffffe0H
	inc	ecx
$LN3@mmap_set:
	mov	edx, 1
	shl	edx, cl
	or	DWORD PTR [eax], edx

; 17   : }

	ret	0
?mmap_set@@YAXH@Z ENDP					; mmap_set
_TEXT	ENDS
PUBLIC	?mmap_unset@@YAXH@Z				; mmap_unset
; Function compile flags: /Ogtpy
;	COMDAT ?mmap_unset@@YAXH@Z
_TEXT	SEGMENT
_bit$ = 8						; size = 4
?mmap_unset@@YAXH@Z PROC				; mmap_unset, COMDAT

; 20   : 	_pmm_mmap[bit/32] = _pmm_mmap[bit/32] & ~(1 << (bit%32));

	mov	ecx, DWORD PTR _bit$[esp-4]
	mov	eax, ecx
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	mov	edx, DWORD PTR __pmm_mmap
	sar	eax, 5
	and	ecx, -2147483617			; 8000001fH
	lea	eax, DWORD PTR [edx+eax*4]
	jns	SHORT $LN3@mmap_unset
	dec	ecx
	or	ecx, -32				; ffffffe0H
	inc	ecx
$LN3@mmap_unset:
	mov	edx, 1
	shl	edx, cl
	not	edx
	and	DWORD PTR [eax], edx

; 21   : }

	ret	0
?mmap_unset@@YAXH@Z ENDP				; mmap_unset
_TEXT	ENDS
PUBLIC	?mmap_test@@YA_NH@Z				; mmap_test
; Function compile flags: /Ogtpy
;	COMDAT ?mmap_test@@YA_NH@Z
_TEXT	SEGMENT
_bit$ = 8						; size = 4
?mmap_test@@YA_NH@Z PROC				; mmap_test, COMDAT

; 24   : 	return _pmm_mmap[bit/32] & (1 << (bit%32));

	mov	eax, DWORD PTR _bit$[esp-4]
	mov	ecx, eax
	and	ecx, -2147483617			; 8000001fH
	push	esi
	jns	SHORT $LN3@mmap_test
	dec	ecx
	or	ecx, -32				; ffffffe0H
	inc	ecx
$LN3@mmap_test:
	cdq
	mov	esi, 1
	shl	esi, cl
	mov	ecx, DWORD PTR __pmm_mmap
	and	edx, 31					; 0000001fH
	add	eax, edx
	sar	eax, 5
	test	esi, DWORD PTR [ecx+eax*4]
	pop	esi
	setne	al

; 25   : }

	ret	0
?mmap_test@@YA_NH@Z ENDP				; mmap_test
_TEXT	ENDS
PUBLIC	?pmm_init@@YAXII@Z				; pmm_init
EXTRN	?memset@@YAPAXPAXDI@Z:PROC			; memset
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_size$ = 8						; size = 4
_mmap_base$ = 12					; size = 4
?pmm_init@@YAXII@Z PROC					; pmm_init

; 75   : 	_pmm_mem_size = size;

	mov	eax, DWORD PTR _size$[esp-4]

; 76   : 	_pmm_mmap = (uint32_t*)mmap_base;

	mov	ecx, DWORD PTR _mmap_base$[esp-4]
	mov	DWORD PTR __pmm_mem_size, eax

; 77   : 	_pmm_max_blocks = _pmm_mem_size*1024 / PMM_BLOCK_SIZE;

	shl	eax, 10					; 0000000aH
	shr	eax, 12					; 0000000cH
	mov	DWORD PTR __pmm_max_blocks, eax

; 78   : 	_pmm_used_blocks = _pmm_max_blocks;  // all memory used by default

	mov	DWORD PTR __pmm_used_blocks, eax

; 79   : 	
; 80   : 	memset(_pmm_mmap,0xf,_pmm_max_blocks/PMM_BLOCK_PER_BYTE);

	shr	eax, 3
	push	eax
	push	15					; 0000000fH
	push	ecx
	mov	DWORD PTR __pmm_mmap, ecx
	call	?memset@@YAPAXPAXDI@Z			; memset
	add	esp, 12					; 0000000cH

; 81   : }

	ret	0
?pmm_init@@YAXII@Z ENDP					; pmm_init
_TEXT	ENDS
PUBLIC	?pmm_init_region@@YAXII@Z			; pmm_init_region
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_base$ = 8						; size = 4
_size$ = 12						; size = 4
?pmm_init_region@@YAXII@Z PROC				; pmm_init_region

; 83   : void pmm_init_region(uint32_t base,size_t size) {

	push	esi

; 84   : 	int align = base/PMM_BLOCK_SIZE;

	mov	esi, DWORD PTR _base$[esp]
	push	edi

; 85   : 	int blocks = size/PMM_BLOCK_SIZE;

	mov	edi, DWORD PTR _size$[esp+4]
	shr	esi, 12					; 0000000cH
	shr	edi, 12					; 0000000cH

; 86   : 	
; 87   : 	for (;blocks >=0;blocks--) {

	js	SHORT $LN12@pmm_init_r
	push	ebx
	mov	ebx, DWORD PTR __pmm_mmap
	npad	7
$LL3@pmm_init_r:

; 88   : 		mmap_unset(align++);

	mov	eax, esi
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	sar	eax, 5
	mov	ecx, esi
	and	ecx, -2147483617			; 8000001fH
	lea	eax, DWORD PTR [ebx+eax*4]
	jns	SHORT $LN13@pmm_init_r
	dec	ecx
	or	ecx, -32				; ffffffe0H
	inc	ecx
$LN13@pmm_init_r:

; 89   : 		_pmm_used_blocks--;

	sub	DWORD PTR __pmm_used_blocks, 1
	mov	edx, 1
	shl	edx, cl
	add	esi, 1
	not	edx
	and	DWORD PTR [eax], edx
	sub	edi, 1
	jns	SHORT $LL3@pmm_init_r

; 90   : 	}
; 91   : 	
; 92   : 	mmap_set(0);

	or	DWORD PTR [ebx], 1
	pop	ebx
	pop	edi
	pop	esi

; 93   : }

	ret	0
$LN12@pmm_init_r:

; 90   : 	}
; 91   : 	
; 92   : 	mmap_set(0);

	mov	eax, DWORD PTR __pmm_mmap
	or	DWORD PTR [eax], 1
	pop	edi
	pop	esi

; 93   : }

	ret	0
?pmm_init_region@@YAXII@Z ENDP				; pmm_init_region
_TEXT	ENDS
PUBLIC	?pmm_deinit_region@@YAXII@Z			; pmm_deinit_region
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_base$ = 8						; size = 4
_size$ = 12						; size = 4
?pmm_deinit_region@@YAXII@Z PROC			; pmm_deinit_region

; 95   : void pmm_deinit_region(uint32_t base,size_t size) {

	push	esi

; 96   : 	int align = base/PMM_BLOCK_SIZE;

	mov	esi, DWORD PTR _base$[esp]
	push	edi

; 97   : 	int blocks = size/PMM_BLOCK_SIZE;

	mov	edi, DWORD PTR _size$[esp+4]
	shr	esi, 12					; 0000000cH
	shr	edi, 12					; 0000000cH

; 98   : 	
; 99   : 	for (;blocks >=0;blocks--) {

	js	SHORT $LN1@pmm_deinit
	mov	eax, DWORD PTR __pmm_used_blocks
	lea	ecx, DWORD PTR [eax+edi+1]
	push	ebx
	mov	ebx, DWORD PTR __pmm_mmap
	mov	DWORD PTR __pmm_used_blocks, ecx
$LL3@pmm_deinit:

; 100  : 		mmap_set(align++);

	mov	eax, esi
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	sar	eax, 5
	mov	ecx, esi
	and	ecx, -2147483617			; 8000001fH
	lea	eax, DWORD PTR [ebx+eax*4]
	jns	SHORT $LN10@pmm_deinit
	dec	ecx
	or	ecx, -32				; ffffffe0H
	inc	ecx
$LN10@pmm_deinit:
	mov	edx, 1
	shl	edx, cl
	add	esi, 1
	or	DWORD PTR [eax], edx
	sub	edi, 1
	jns	SHORT $LL3@pmm_deinit
	pop	ebx
$LN1@pmm_deinit:
	pop	edi
	pop	esi

; 101  : 		_pmm_used_blocks++;
; 102  : 	}
; 103  : }

	ret	0
?pmm_deinit_region@@YAXII@Z ENDP			; pmm_deinit_region
_TEXT	ENDS
PUBLIC	?pmm_dealloc@@YAXPAX@Z				; pmm_dealloc
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_p$ = 8							; size = 4
?pmm_dealloc@@YAXPAX@Z PROC				; pmm_dealloc

; 141  : 	uint32_t addr = (uint32_t)p;
; 142  : 	int block = addr / PMM_BLOCK_SIZE;

	mov	ecx, DWORD PTR _p$[esp-4]
	shr	ecx, 12					; 0000000cH

; 143  : 	mmap_unset(block);

	mov	eax, ecx
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	mov	edx, DWORD PTR __pmm_mmap
	sar	eax, 5
	and	ecx, -2147483617			; 8000001fH
	lea	eax, DWORD PTR [edx+eax*4]
	jns	SHORT $LN5@pmm_deallo
	dec	ecx
	or	ecx, -32				; ffffffe0H
	inc	ecx
$LN5@pmm_deallo:

; 144  : 	_pmm_used_blocks--;

	sub	DWORD PTR __pmm_used_blocks, 1
	mov	edx, 1
	shl	edx, cl
	not	edx
	and	DWORD PTR [eax], edx

; 145  : }

	ret	0
?pmm_dealloc@@YAXPAX@Z ENDP				; pmm_dealloc
_TEXT	ENDS
PUBLIC	?pmm_deallocs@@YAXPAXI@Z			; pmm_deallocs
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_p$ = 8							; size = 4
_s$ = 12						; size = 4
?pmm_deallocs@@YAXPAXI@Z PROC				; pmm_deallocs

; 147  : void pmm_deallocs(void* p,size_t s) {

	push	ebp

; 148  : 	uint32_t addr = (uint32_t)p;
; 149  : 	int block = addr / PMM_BLOCK_SIZE;
; 150  : 	
; 151  : 	for (uint32_t i=0;i<s;++i) 

	mov	ebp, DWORD PTR _s$[esp]
	push	esi
	mov	esi, DWORD PTR _p$[esp+4]
	shr	esi, 12					; 0000000cH
	test	ebp, ebp
	jbe	SHORT $LN10@pmm_deallo@2
	push	ebx
	mov	ebx, DWORD PTR __pmm_mmap
	push	edi
	mov	edi, ebp
	npad	5
$LL3@pmm_deallo@2:

; 152  : 		mmap_unset(block+i);

	mov	eax, esi
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	sar	eax, 5
	mov	ecx, esi
	and	ecx, -2147483617			; 8000001fH
	lea	eax, DWORD PTR [ebx+eax*4]
	jns	SHORT $LN11@pmm_deallo@2
	dec	ecx
	or	ecx, -32				; ffffffe0H
	inc	ecx
$LN11@pmm_deallo@2:
	mov	edx, 1
	shl	edx, cl
	add	esi, 1
	not	edx
	and	DWORD PTR [eax], edx
	sub	edi, 1
	jne	SHORT $LL3@pmm_deallo@2
	pop	edi
	pop	ebx
$LN10@pmm_deallo@2:

; 153  : 		
; 154  : 	_pmm_used_blocks -= s;	

	sub	DWORD PTR __pmm_used_blocks, ebp
	pop	esi
	pop	ebp

; 155  : }

	ret	0
?pmm_deallocs@@YAXPAXI@Z ENDP				; pmm_deallocs
_TEXT	ENDS
PUBLIC	?pmm_get_mem_size@@YAIXZ			; pmm_get_mem_size
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
?pmm_get_mem_size@@YAIXZ PROC				; pmm_get_mem_size

; 158  : 	return _pmm_mem_size;

	mov	eax, DWORD PTR __pmm_mem_size

; 159  : }

	ret	0
?pmm_get_mem_size@@YAIXZ ENDP				; pmm_get_mem_size
_TEXT	ENDS
PUBLIC	?pmm_get_block_count@@YAIXZ			; pmm_get_block_count
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
?pmm_get_block_count@@YAIXZ PROC			; pmm_get_block_count

; 162  : 	return _pmm_max_blocks;

	mov	eax, DWORD PTR __pmm_max_blocks

; 163  : }

	ret	0
?pmm_get_block_count@@YAIXZ ENDP			; pmm_get_block_count
_TEXT	ENDS
PUBLIC	?pmm_get_used_block_count@@YAIXZ		; pmm_get_used_block_count
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
?pmm_get_used_block_count@@YAIXZ PROC			; pmm_get_used_block_count

; 166  : 	return _pmm_used_blocks;

	mov	eax, DWORD PTR __pmm_used_blocks

; 167  : }

	ret	0
?pmm_get_used_block_count@@YAIXZ ENDP			; pmm_get_used_block_count
_TEXT	ENDS
PUBLIC	?pmm_get_free_block_count@@YAIXZ		; pmm_get_free_block_count
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
?pmm_get_free_block_count@@YAIXZ PROC			; pmm_get_free_block_count

; 170  : 	return _pmm_max_blocks - _pmm_used_blocks;

	mov	eax, DWORD PTR __pmm_max_blocks
	sub	eax, DWORD PTR __pmm_used_blocks

; 171  : }

	ret	0
?pmm_get_free_block_count@@YAIXZ ENDP			; pmm_get_free_block_count
_TEXT	ENDS
PUBLIC	?pmm_get_block_size@@YAIXZ			; pmm_get_block_size
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
?pmm_get_block_size@@YAIXZ PROC				; pmm_get_block_size

; 174  : 	return PMM_BLOCK_SIZE;

	mov	eax, 4096				; 00001000H

; 175  : }

	ret	0
?pmm_get_block_size@@YAIXZ ENDP				; pmm_get_block_size
_TEXT	ENDS
PUBLIC	?pmm_enable_paging@@YAX_N@Z			; pmm_enable_paging
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_val$ = 8						; size = 1
?pmm_enable_paging@@YAX_N@Z PROC			; pmm_enable_paging

; 177  : void pmm_enable_paging(bool val) {

	push	ebp
	mov	ebp, esp

; 178  : #ifdef _MSC_VER
; 179  : 	_asm {
; 180  : 	
; 181  : 		mov eax,cr0

	mov	eax, cr0

; 182  : 		cmp [val],1

	cmp	BYTE PTR _val$[ebp], 1

; 183  : 		je enable

	je	SHORT $enable$2721

; 184  : 		jmp disable

	jmp	SHORT $disable$2722
$enable$2721:

; 185  : 	
; 186  : 	enable:
; 187  : 		or eax,0x80000000  // set last bit

	or	eax, -2147483648			; 80000000H

; 188  : 		mov cr0,eax

	mov	cr0, eax

; 189  : 		jmp done

	jmp	SHORT $done$2723
$disable$2722:

; 190  : 		
; 191  : 	disable:
; 192  : 		and eax,0x7fffffff // unset last bit

	and	eax, 2147483647				; 7fffffffH

; 193  : 		mov cr0,eax

	mov	cr0, eax
$done$2723:

; 194  : 	
; 195  : 	done:
; 196  : 	}
; 197  : 	
; 198  : #endif
; 199  : }

	pop	ebp
	ret	0
?pmm_enable_paging@@YAX_N@Z ENDP			; pmm_enable_paging
_TEXT	ENDS
PUBLIC	?pmm_is_paging@@YA_NXZ				; pmm_is_paging
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_val$ = -4						; size = 4
?pmm_is_paging@@YA_NXZ PROC				; pmm_is_paging

; 201  : bool pmm_is_paging() {

	push	ecx

; 202  : 	uint32_t val = 0;

	mov	DWORD PTR _val$[esp+4], 0

; 203  : #ifdef _MSC_VER
; 204  : 	_asm {
; 205  : 		mov eax,cr0

	mov	eax, cr0

; 206  : 		mov [val],eax

	mov	DWORD PTR _val$[esp+4], eax

; 207  : 	}
; 208  : 	
; 209  : 	if ( val & 0x80000000 )
; 210  : 		false;
; 211  : 	else
; 212  : 		true;
; 213  : #endif
; 214  : }

	pop	ecx
	ret	0
?pmm_is_paging@@YA_NXZ ENDP				; pmm_is_paging
_TEXT	ENDS
PUBLIC	?pmm_load_PDBR@@YAXI@Z				; pmm_load_PDBR
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_addr$ = 8						; size = 4
?pmm_load_PDBR@@YAXI@Z PROC				; pmm_load_PDBR

; 217  : #ifdef _MSC_VER
; 218  : 	_asm {
; 219  : 		mov eax,[addr]

	mov	eax, DWORD PTR _addr$[esp-4]

; 220  : 		mov cr3,eax

	mov	cr3, eax

; 221  : 	}
; 222  : #endif
; 223  : }

	ret	0
?pmm_load_PDBR@@YAXI@Z ENDP				; pmm_load_PDBR
_TEXT	ENDS
PUBLIC	?pmm_get_PDBR@@YAIXZ				; pmm_get_PDBR
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
?pmm_get_PDBR@@YAIXZ PROC				; pmm_get_PDBR

; 226  : #ifdef _MSC_VER
; 227  : 	_asm {
; 228  : 		mov eax,cr3

	mov	eax, cr3

; 229  : 		ret

	ret	0

; 230  : 	}
; 231  : #endif
; 232  : }

	ret	0
?pmm_get_PDBR@@YAIXZ ENDP				; pmm_get_PDBR
_TEXT	ENDS
PUBLIC	?mmap_find_first@@YAHXZ				; mmap_find_first
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
?mmap_find_first@@YAHXZ PROC				; mmap_find_first

; 27   : int mmap_find_first() {

	push	ebx
	push	esi

; 28   : 	
; 29   : 	for (uint32_t i=0;i<pmm_get_free_block_count()/32;++i) {

	mov	esi, DWORD PTR __pmm_max_blocks
	sub	esi, DWORD PTR __pmm_used_blocks
	xor	eax, eax
	shr	esi, 5
	push	edi
	je	SHORT $LN6@mmap_find_
	mov	edi, DWORD PTR __pmm_mmap
	npad	4
$LL8@mmap_find_:

; 30   : 		if (_pmm_mmap[i] != 0xffffffff) {

	mov	edx, DWORD PTR [edi+eax*4]
	cmp	edx, -1
	je	SHORT $LN7@mmap_find_

; 31   : 			for (int j=0;j<32;++j) {

	xor	ecx, ecx
	npad	6
$LL4@mmap_find_:

; 32   : 				int bit = 1 << j;

	mov	ebx, 1
	shl	ebx, cl

; 33   : 				if (!(_pmm_mmap[i] & bit))

	test	ebx, edx
	je	SHORT $LN15@mmap_find_
	add	ecx, 1
	cmp	ecx, 32					; 00000020H
	jl	SHORT $LL4@mmap_find_
$LN7@mmap_find_:

; 28   : 	
; 29   : 	for (uint32_t i=0;i<pmm_get_free_block_count()/32;++i) {

	add	eax, 1
	cmp	eax, esi
	jb	SHORT $LL8@mmap_find_
$LN6@mmap_find_:
	pop	edi
	pop	esi

; 35   : 			}
; 36   : 		}
; 37   : 	}
; 38   : 	
; 39   : 	return -1;

	or	eax, -1
	pop	ebx

; 40   : }

	ret	0
$LN15@mmap_find_:
	pop	edi

; 34   : 					return i*32+j;

	shl	eax, 5
	pop	esi
	add	eax, ecx
	pop	ebx

; 40   : }

	ret	0
?mmap_find_first@@YAHXZ ENDP				; mmap_find_first
_TEXT	ENDS
PUBLIC	?mmap_find_squence@@YAHI@Z			; mmap_find_squence
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_i$2630 = -20						; size = 4
_j$2635 = -16						; size = 4
tv282 = -12						; size = 4
$T2822 = -8						; size = 4
tv201 = -4						; size = 4
tv278 = 8						; size = 4
_s$ = 8							; size = 4
?mmap_find_squence@@YAHI@Z PROC				; mmap_find_squence

; 42   : int mmap_find_squence(size_t s) {

	sub	esp, 20					; 00000014H
	push	ebp

; 43   : 	if (s == 0)

	mov	ebp, DWORD PTR _s$[esp+20]
	xor	eax, eax
	cmp	ebp, eax
	jne	SHORT $LN15@mmap_find_@2

; 44   : 		return -1;

	or	eax, -1
	pop	ebp

; 71   : }

	add	esp, 20					; 00000014H
	ret	0
$LN15@mmap_find_@2:

; 45   : 	
; 46   : 	if (s == 1)

	cmp	ebp, 1
	jne	SHORT $LN14@mmap_find_@2
	pop	ebp

; 71   : }

	add	esp, 20					; 00000014H

; 47   : 		return mmap_find_first();

	jmp	?mmap_find_first@@YAHXZ			; mmap_find_first
$LN14@mmap_find_@2:
	push	ebx
	push	esi

; 48   : 		
; 49   : 	for (uint32_t i=0;i<pmm_get_free_block_count()/32;++i) {

	mov	esi, DWORD PTR __pmm_max_blocks
	sub	esi, DWORD PTR __pmm_used_blocks
	push	edi
	shr	esi, 5
	mov	DWORD PTR _i$2630[esp+36], eax
	mov	DWORD PTR $T2822[esp+36], esi
	je	$LN31@mmap_find_@2
	mov	DWORD PTR tv278[esp+32], eax
	npad	8
$LL33@mmap_find_@2:

; 50   : 		if (_pmm_mmap[i] != 0xffffffff) {

	mov	ecx, DWORD PTR __pmm_mmap
	mov	edx, DWORD PTR [ecx+eax*4]
	cmp	edx, -1
	mov	DWORD PTR tv201[esp+36], edx
	je	$LN12@mmap_find_@2

; 51   : 			for (int j=0;j<32;++j) {

	xor	ecx, ecx
	mov	DWORD PTR _j$2635[esp+36], ecx
	npad	4
$LL32@mmap_find_@2:

; 52   : 				int bit = 1 << j;

	mov	eax, 1
	shl	eax, cl

; 53   : 				if (!(_pmm_mmap[i] & bit)) {

	test	edx, eax
	jne	SHORT $LN8@mmap_find_@2
	mov	edx, DWORD PTR tv278[esp+32]

; 54   : 					
; 55   : 					int start_bit = i*32 + bit;
; 56   : 					uint32_t free_bit = 0;

	xor	ebx, ebx
	lea	esi, DWORD PTR [edx+eax]

; 57   : 					
; 58   : 					for (uint32_t count=0;count<=s;++count) {

	xor	edi, edi
$LL29@mmap_find_@2:

; 59   : 						if (!(mmap_test(start_bit+count)))

	mov	ecx, esi
	and	ecx, -2147483617			; 8000001fH
	jns	SHORT $LN38@mmap_find_@2
	dec	ecx
	or	ecx, -32				; ffffffe0H
	inc	ecx
$LN38@mmap_find_@2:
	mov	eax, 1
	shl	eax, cl
	mov	ecx, DWORD PTR __pmm_mmap
	mov	DWORD PTR tv282[esp+36], eax
	mov	eax, esi
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	mov	edx, DWORD PTR tv282[esp+36]
	sar	eax, 5
	test	edx, DWORD PTR [ecx+eax*4]
	jne	SHORT $LN30@mmap_find_@2

; 60   : 							free_bit++;

	add	ebx, 1
$LN30@mmap_find_@2:

; 61   : 							
; 62   : 						if (free_bit == s)

	cmp	ebx, ebp
	je	SHORT $LN25@mmap_find_@2
	add	edi, 1
	add	esi, 1
	cmp	edi, ebp
	jbe	SHORT $LL29@mmap_find_@2
	mov	ecx, DWORD PTR _j$2635[esp+36]
	mov	esi, DWORD PTR $T2822[esp+36]
	mov	edx, DWORD PTR tv201[esp+36]
$LN8@mmap_find_@2:
	add	ecx, 1
	cmp	ecx, 32					; 00000020H
	mov	DWORD PTR _j$2635[esp+36], ecx
	jl	SHORT $LL32@mmap_find_@2

; 51   : 			for (int j=0;j<32;++j) {

	mov	eax, DWORD PTR _i$2630[esp+36]
$LN12@mmap_find_@2:

; 48   : 		
; 49   : 	for (uint32_t i=0;i<pmm_get_free_block_count()/32;++i) {

	add	DWORD PTR tv278[esp+32], 32		; 00000020H
	add	eax, 1
	cmp	eax, esi
	mov	DWORD PTR _i$2630[esp+36], eax
	jb	$LL33@mmap_find_@2
$LN31@mmap_find_@2:
	pop	edi
	pop	esi
	pop	ebx

; 64   : 					}
; 65   : 				}
; 66   : 			}
; 67   : 		}
; 68   : 	}
; 69   : 	
; 70   : 	return -1;

	or	eax, -1
	pop	ebp

; 71   : }

	add	esp, 20					; 00000014H
	ret	0
$LN25@mmap_find_@2:

; 63   : 							return i*32+j;

	mov	eax, DWORD PTR _i$2630[esp+36]
	pop	edi
	pop	esi
	shl	eax, 5
	add	eax, DWORD PTR _j$2635[esp+28]
	pop	ebx
	pop	ebp

; 71   : }

	add	esp, 20					; 00000014H
	ret	0
?mmap_find_squence@@YAHI@Z ENDP				; mmap_find_squence
_TEXT	ENDS
PUBLIC	?pmm_alloc@@YAPAXXZ				; pmm_alloc
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
?pmm_alloc@@YAPAXXZ PROC				; pmm_alloc

; 106  : 	if (pmm_get_free_block_count() <= 0)

	mov	eax, DWORD PTR __pmm_max_blocks
	sub	eax, DWORD PTR __pmm_used_blocks
	jne	SHORT $LN2@pmm_alloc

; 107  : 		return 0;

	xor	eax, eax

; 118  : 	
; 119  : 	return (void*) addr;
; 120  : }

	ret	0
$LN2@pmm_alloc:
	push	esi

; 108  : 		
; 109  : 	int block = mmap_find_first();

	call	?mmap_find_first@@YAHXZ			; mmap_find_first
	mov	esi, eax

; 110  : 	
; 111  : 	if (block == -1)

	cmp	esi, -1
	jne	SHORT $LN1@pmm_alloc

; 112  : 		return 0;

	xor	eax, eax
	pop	esi

; 118  : 	
; 119  : 	return (void*) addr;
; 120  : }

	ret	0
$LN1@pmm_alloc:

; 113  : 		
; 114  : 	mmap_set(block);

	mov	ecx, DWORD PTR __pmm_mmap
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	sar	eax, 5
	lea	eax, DWORD PTR [ecx+eax*4]
	mov	ecx, esi
	and	ecx, -2147483617			; 8000001fH
	jns	SHORT $LN10@pmm_alloc
	dec	ecx
	or	ecx, -32				; ffffffe0H
	inc	ecx
$LN10@pmm_alloc:

; 115  : 	
; 116  : 	uint32_t addr = block * PMM_BLOCK_SIZE;
; 117  : 	_pmm_used_blocks++;

	add	DWORD PTR __pmm_used_blocks, 1
	mov	edx, 1
	shl	edx, cl
	or	DWORD PTR [eax], edx
	mov	eax, esi
	shl	eax, 12					; 0000000cH
	pop	esi

; 118  : 	
; 119  : 	return (void*) addr;
; 120  : }

	ret	0
?pmm_alloc@@YAPAXXZ ENDP				; pmm_alloc
_TEXT	ENDS
PUBLIC	?pmm_allocs@@YAPAXI@Z				; pmm_allocs
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_block$ = 8						; size = 4
_s$ = 8							; size = 4
?pmm_allocs@@YAPAXI@Z PROC				; pmm_allocs

; 123  : 	if (pmm_get_free_block_count() <= s)

	mov	eax, DWORD PTR __pmm_max_blocks
	sub	eax, DWORD PTR __pmm_used_blocks
	push	ebp
	mov	ebp, DWORD PTR _s$[esp]
	cmp	eax, ebp
	ja	SHORT $LN5@pmm_allocs
$LN15@pmm_allocs:

; 124  : 		return 0;

	xor	eax, eax
	pop	ebp

; 136  : 	
; 137  : 	return (void*) addr;
; 138  : }

	ret	0
$LN5@pmm_allocs:

; 125  : 		
; 126  : 	int block = mmap_find_squence(s);

	push	ebp
	call	?mmap_find_squence@@YAHI@Z		; mmap_find_squence
	add	esp, 4

; 127  : 	
; 128  : 	if (block == -1)

	cmp	eax, -1
	mov	DWORD PTR _block$[esp], eax

; 129  : 		return 0;	

	je	SHORT $LN15@pmm_allocs

; 130  : 		
; 131  : 	for (uint32_t i=0;i<s;++i) 

	test	ebp, ebp
	jbe	SHORT $LN1@pmm_allocs
	push	ebx
	mov	ebx, DWORD PTR __pmm_mmap
	push	esi
	push	edi
	mov	esi, eax
	mov	edi, ebp
	npad	5
$LL3@pmm_allocs:

; 132  : 		mmap_set(block+i);

	mov	eax, esi
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	sar	eax, 5
	mov	ecx, esi
	and	ecx, -2147483617			; 8000001fH
	lea	eax, DWORD PTR [ebx+eax*4]
	jns	SHORT $LN14@pmm_allocs
	dec	ecx
	or	ecx, -32				; ffffffe0H
	inc	ecx
$LN14@pmm_allocs:
	mov	edx, 1
	shl	edx, cl
	add	esi, 1
	or	DWORD PTR [eax], edx
	sub	edi, 1
	jne	SHORT $LL3@pmm_allocs
	mov	eax, DWORD PTR _block$[esp+12]
	pop	edi
	pop	esi
	pop	ebx
$LN1@pmm_allocs:

; 133  : 	
; 134  : 	uint32_t addr = block * PMM_BLOCK_SIZE;
; 135  : 	_pmm_used_blocks += s;

	add	DWORD PTR __pmm_used_blocks, ebp
	shl	eax, 12					; 0000000cH
	pop	ebp

; 136  : 	
; 137  : 	return (void*) addr;
; 138  : }

	ret	0
?pmm_allocs@@YAPAXI@Z ENDP				; pmm_allocs
_TEXT	ENDS
END
