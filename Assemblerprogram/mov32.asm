BITS 32

global _start

section .text

_start:

	mov ax, 0x2222
	shl eax, 0x10
	mov ax, 0x1111
	nop
	mov ecx, 0x22221111	
	nop
