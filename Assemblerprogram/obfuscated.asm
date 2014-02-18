section .data
	shellpath: db "/bin/sh",0
	shellpathlen: equ $-shellpath

section	.text
	global	_start

_start:

	lea esi, [shellpath]

	jmp stuff+4

stuff:
	cmovo ax, [ecx+esi-0x406f9940]
	o16 seto byte [ecx+ecx*4-0x406ff78a]
	lea ax, [ecx+ecx*4-0x406f63ba]
	o16 seto byte [ebp+ecx*4-0x406f99e2]
	cmovo ax, [ebp+ecx*4-0x406ff7b2]
	cmovo ax, [ebp+ecx*4-0x406ff3aa]
	o16 seto byte [ebp+ecx*8-0x6f999980]

	
	mov ebx,0x00
	mov eax,0x01
	int 0x80
	