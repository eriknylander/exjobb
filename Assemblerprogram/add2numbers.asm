
extern printf
global main
section .data
	fmt db 'The sum of %d and %d is',0xA,0

section .text
	
main:
	pop ebx ; number of arguments
	;pop ebx ; program name
	;pop	ebx ; argument 1
	;pop ecx ; argument 2

	;xor edx, edx

	;add edx, ebx
	;add edx, ecx

	;sub esp, 12

	;push ebx
	;push ecx
	;push edx

	;call printf

	;add esp, 12

	;ret