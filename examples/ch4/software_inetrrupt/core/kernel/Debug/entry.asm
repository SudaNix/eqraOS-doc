; Listing generated by Microsoft (R) Optimizing Compiler Version 14.00.50727.42 

	TITLE	c:\Documents and Settings\SudaNix\Desktop\docs\research\examples\ch4\software_inetrrupt\core\kernel\entry.cpp
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB LIBCMT
INCLUDELIB OLDNAMES

PUBLIC	?kernel_entry@@YAXXZ				; kernel_entry
EXTRN	?exit@@YAXXZ:PROC				; exit
EXTRN	_main:PROC
EXTRN	?init_ctor@@YAXXZ:PROC				; init_ctor
; Function compile flags: /Ogtpy
; File c:\documents and settings\sudanix\desktop\docs\research\examples\ch4\software_inetrrupt\core\kernel\entry.cpp
_TEXT	SEGMENT
?kernel_entry@@YAXXZ PROC				; kernel_entry

; 15   : 
; 16   : #ifdef i386
; 17   : 	_asm {
; 18   : 		cli						

	cli

; 19   : 		
; 20   : 		mov ax, 10h				// select data descriptor in GDT.

	mov	ax, 16					; 00000010H

; 21   : 		mov ds, ax

	mov	ds, ax

; 22   : 		mov es, ax

	mov	es, ax

; 23   : 		mov fs, ax

	mov	fs, ax

; 24   : 		mov gs, ax

	mov	gs, ax

; 25   : 		//mov ss, ax				// Set up base stack
; 26   : 		//mov esp, 0x90000
; 27   : 		//mov ebp, esp			// store current stack pointer
; 28   : 		//push ebp
; 29   : 	}
; 30   : #endif
; 31   : 
; 32   : 	// Execute global constructors
; 33   : 	init_ctor();

	call	?init_ctor@@YAXXZ			; init_ctor

; 34   : 
; 35   : 	// Call kernel entry point
; 36   : 	main();

	call	_main

; 37   : 
; 38   : 	// Cleanup all dynamic dtors
; 39   : 	exit();

	call	?exit@@YAXXZ				; exit

; 43   : 		cli

	cli

; 44   : 		hlt

	hlt

; 40   : 
; 41   : #ifdef i386
; 42   : 	_asm {

$LL2@kernel_ent:

; 45   : 	}
; 46   : #endif
; 47   : 
; 48   : 	for(;;);

	jmp	SHORT $LL2@kernel_ent
?kernel_entry@@YAXXZ ENDP				; kernel_entry
_TEXT	ENDS
END