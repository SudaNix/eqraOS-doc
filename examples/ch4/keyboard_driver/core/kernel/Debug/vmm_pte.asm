; Listing generated by Microsoft (R) Optimizing Compiler Version 14.00.50727.42 

	TITLE	c:\Documents and Settings\SudaNix\Desktop\eqraOS\eqraOS-doc\examples\ch4\keyboard_driver\core\kernel\vmm_pte.cpp
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB LIBCMT
INCLUDELIB OLDNAMES

PUBLIC	?pte_add_attrib@@YAXPAII@Z			; pte_add_attrib
; Function compile flags: /Ogtpy
; File c:\documents and settings\sudanix\desktop\eqraos\eqraos-doc\examples\ch4\keyboard_driver\core\kernel\vmm_pte.cpp
_TEXT	SEGMENT
_e$ = 8							; size = 4
_attrib$ = 12						; size = 4
?pte_add_attrib@@YAXPAII@Z PROC				; pte_add_attrib

; 4    : 	*e = *e | attrib; 

	mov	eax, DWORD PTR _e$[esp-4]
	mov	ecx, DWORD PTR _attrib$[esp-4]
	or	DWORD PTR [eax], ecx

; 5    : }

	ret	0
?pte_add_attrib@@YAXPAII@Z ENDP				; pte_add_attrib
_TEXT	ENDS
PUBLIC	?pte_del_attrib@@YAXPAII@Z			; pte_del_attrib
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_e$ = 8							; size = 4
_attrib$ = 12						; size = 4
?pte_del_attrib@@YAXPAII@Z PROC				; pte_del_attrib

; 8    : 	*e = *e & ~attrib;

	mov	ecx, DWORD PTR _attrib$[esp-4]
	mov	eax, DWORD PTR _e$[esp-4]
	not	ecx
	and	DWORD PTR [eax], ecx

; 9    : }

	ret	0
?pte_del_attrib@@YAXPAII@Z ENDP				; pte_del_attrib
_TEXT	ENDS
PUBLIC	?pte_set_frame@@YAXPAII@Z			; pte_set_frame
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_e$ = 8							; size = 4
_addr$ = 12						; size = 4
?pte_set_frame@@YAXPAII@Z PROC				; pte_set_frame

; 12   : 	*e = ( *e & ~I386_PTE_FRAME ) | addr ;

	mov	eax, DWORD PTR _e$[esp-4]
	mov	ecx, DWORD PTR [eax]
	and	ecx, -2147479553			; 80000fffH
	or	ecx, DWORD PTR _addr$[esp-4]
	mov	DWORD PTR [eax], ecx

; 13   : }

	ret	0
?pte_set_frame@@YAXPAII@Z ENDP				; pte_set_frame
_TEXT	ENDS
PUBLIC	?pte_get_frame@@YAII@Z				; pte_get_frame
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_e$ = 8							; size = 4
?pte_get_frame@@YAII@Z PROC				; pte_get_frame

; 16   : 	return e & I386_PTE_FRAME ;

	mov	eax, DWORD PTR _e$[esp-4]
	and	eax, 2147479552				; 7ffff000H

; 17   : }

	ret	0
?pte_get_frame@@YAII@Z ENDP				; pte_get_frame
_TEXT	ENDS
PUBLIC	?pte_is_present@@YA_NI@Z			; pte_is_present
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_e$ = 8							; size = 4
?pte_is_present@@YA_NI@Z PROC				; pte_is_present

; 20   : 	return e & I386_PTE_PRESENT;

	mov	eax, DWORD PTR _e$[esp-4]
	and	eax, 1

; 21   : }

	ret	0
?pte_is_present@@YA_NI@Z ENDP				; pte_is_present
_TEXT	ENDS
PUBLIC	?pte_is_writable@@YA_NI@Z			; pte_is_writable
; Function compile flags: /Ogtpy
_TEXT	SEGMENT
_e$ = 8							; size = 4
?pte_is_writable@@YA_NI@Z PROC				; pte_is_writable

; 24   : 	return e & I386_PTE_WRITABLE ;

	mov	eax, DWORD PTR _e$[esp-4]
	shr	eax, 1
	and	al, 1

; 25   : }

	ret	0
?pte_is_writable@@YA_NI@Z ENDP				; pte_is_writable
_TEXT	ENDS
END
