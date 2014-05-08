BITS 32

section .data

textoutput db 'Hello', 10
textoutputlen equ $ - textoutput
filetoopen db 'hello.txt', 0

section .text
global _start

_start:
	nop
	mov eax, 0x5
	mov ebx, filetoopen
	mov ecx, 0x2
	int 0x80

	cmp eax, 0
	jl 0x13

	mov ebx, eax
	mov eax, 0x4
	mov ecx, textoutput
	mov edx, textoutputlen
	int 0x80


exit:
	mov eax, 1
	mov ebx, 0
	int 0x80

	mov eax, 0x12345678
