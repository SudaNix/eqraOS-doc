;Hello Bootloader

bits 16				; 16-bit real mode.
org	 0x0			; this number will added to all addresses (relocating).

start:
		jmp main	; jump over data and function to entry point.
		
		
;******************************************
; OEM Id and BIOS Parameter Block (BPB) 
;******************************************

; must begin at byte 3(4th byte), if not we should add nop instruction.

OEM_ID               db        "eqraOS  "    ; Name of your OS, Must be 8 byte! no more no less.

bytes_per_sector     dw        0x200		; 512 byte per sector.
sectors_per_cluster  db        0x1          ; 1 sector per cluster.
reserved_sectors     dw        0x1          ; boot sector is reserved.
total_fats           db        0x2          ; two fats.
root_directory       dw        0xe0         ; root dir has 224 entries.
total_sectors        dw        0xb40        ; 2880 sectors in the volume.
media_descriptor     db        0xf0         ; 1.44 floppy disk.
sectors_per_fat      dw        0x9          ; 9 sector per fat.
sectors_per_track    dw        0x12         ; 18 sector per track.
number_of_heads      dw        0x2          ; 2 heads per platter.
hidden_sectors       dd        0x0          ; no hidden sector.
total_sectors_large  dd        0x0

; Extended BPB.

drive_number         db        0x0
flags                db        0x0
signature            db        0x29         ; must be 0x28 or 0x29.
volume_id            dd        0x0          ; serial number written when foramt the disk.
volume_label         db        "MOS FLOPPY " ; 11 byte.
system_id            db        "fat12   "    ;  8 byte.


; ****************
;	data
; ****************

hello_msg		db		"Welcome to eqraOS, Coded by Ahmad Essam",0xa,0xd,0

; *********************************************
;	puts16: prints string using BIOS interrupt
;   input:
;				es: pointer to data segment.
;				si: point to the string
; *********************************************

puts16:
		
		lodsb			; read character from ds:si to al ,and increment si if df=0.
		
		cmp al,0		; check end of string ?
		je end_puts16	; yes jump to end.
		
		mov ah,0xe		; print character routine number.
		int 0x10		; call BIOS.
		
		jmp puts16		; continue prints until 0 is found.
		
	end_puts16:
	
		ret				
		
		
; ***************************************
;		entry point of bootloader.
; ***************************************
		
main:				

		;---------------------
		;	intit registers
		;---------------------
		
		; because bootloader are loaded at 0x07c00 we can refrence this location with many different combination
		; of segment:offset addressing.
		
		; So we will use either 0x0000:0x7c000 or 0x07c0:0x0000
		; and in this example we use 0x07c0 for segment and 0x0 for offset.
		
		mov ax,0x07c0			
		mov ds,ax
		mov es,ax
		
		mov si,hello_msg
		call puts16

		cli			; clear interrupt.		
		hlt			; halt the system.
		
		times	510-($-$$)	db		0		; append zeros.
		
		; finally the boot signature 0xaa55
		db		0x55
		db		0xaa
		