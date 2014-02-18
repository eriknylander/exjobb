section .data
virtual db 'I am running on a virtual machine',0xa,0
virtuallen equ $-virtual
nonvirtual: db 'I am not running on a virtual machine',0xa,0
nonvirtuallen equ $-nonvirtual
idtaddr DD 0x0

section .text
	global _start

_start:

	mov eax, 0x1
	sidt [idtaddr]

	mov eax, idtaddr
	mov ecx, 0xd0000000

	

	cmp eax, ecx
	jg printnonvirtual

	mov eax, 0x4
	mov edx, 0x1
	mov ecx, virtual
	int 0x80
	
	


printnonvirtual:
	mov eax, 0x4
	mov edx, nonvirtuallen
	mov ecx, nonvirtual
	int 0x80

	mov ebx,0x00
	mov eax,0x01
	int 0x80