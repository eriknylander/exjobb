BITS 32

section .data
	shellpath: db "/bin/sh",0
	shellpathlen: equ $-shellpath

section .text
	global _start

_start:

	lea esi, [shellpath]
	
	
	xor eax, eax

	mov dword [esi+shellpathlen], esi
	mov dword [esi+shellpathlen+4], eax 

	mov eax, 0xb
	lea ebx, [esi]
	lea ecx, [esi+shellpathlen]
	lea edx, [esi+shellpathlen+4]
	int 0x80
