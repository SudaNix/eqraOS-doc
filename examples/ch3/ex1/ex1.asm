;Simple Bootloader do nothing.

bits 16				; 16-bit real mode.

start:				; label are pointer.
		cli			; clear interrupt.		
		hlt			; halt the system.
		
		times	510-($-$$)	db		0		; append zeros.
		
		; 	$ is the address of first instruction (should be 0x07c00).
		;	$$ is the address of current line.
		; 	$-$$ means how many byte between start and current.
		
		; if cli and hlt take 4 byte then time directive will fill
		; 510-4 = 506 zero's.
		
		; finally the boot signature 0xaa55
		db		0x55	; first byte of a boot signature.
		db		0xaa	; second byte of a  boot signature.