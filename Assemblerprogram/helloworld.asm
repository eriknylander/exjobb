BITS 32
section	.data
hello	db	"Hello world", 0x0a
hellolen	equ	$-hello

section	.text
	global	_start

_start:
	
	mov	ebx,0x01
	mov	ecx,hello
	mov edx,hellolen
	mov eax,0x04
	int 0x80

	mov ebx,0x00
	mov eax,0x01
	int 0x80
