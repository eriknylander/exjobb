BITS 32

global _start

section .text

_start:

	mov ax, 0x0
	shl eax, 0x10
	mov ax, 0xb
	nop
	mov ecx, 0xb	
	nop
