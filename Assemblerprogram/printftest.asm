global main

extern printf

section .data
	fmtString: db 'hello fucking world! Another string is %s"', 0xA,0
	string:	db	'"dear lord y u no work!?!', 0
	fmtStringLen equ $-fmtString

section	.text
	main:

	push string

	push fmtString

	call printf

	pop ebx
	pop ebx
	

	ret

