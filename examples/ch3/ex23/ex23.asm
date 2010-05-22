
%define VIDEO_MEMORY	0xb8000		; Base Address of Mapped Video Memory.
%define CHAR_ATTRIBUTE	0x7			; White chracter on black background.

mov edi,VIDEO_MEMORY

mov [edi],'A'						; print A
mov [edi+1],CHAR_ATTRIBUTE			; in white foreground black background.
