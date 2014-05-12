BITS 32
section .data
;ALLOCATE DATA HERE
;Don't forget data for memory access adjustment

	textoutput db 'Hello', 10
	textoutputlen equ $ - textoutput
	filetoopen db 'hello.txt', 0
	
	ediData: dd 1
	eaxData: dd 1
	ecxData: dd 1


section .text
	global _start
_start:
;preface

;Change 0xdeadc0de to 'label - 1'

;Memory adjustment stuff (Change 0xdeadc0de to according memory access adjusment label)
lea edi, [ediData]
sub edi, 0x5b890ff
lea eax, [eaxData]
sub eax, 0x0
lea ecx, [ecxData]
sub ecx, 0x2

;jmp mep0 + 0x3

;mep
mep0:
test dword [edi+0x5b890ff], 0xbb000000
inc esi
add [eax], al
add [ecx+0x2], bh
int 0x80
cmp eax, 0x0
jl 0x2d6d57e9
mov ebx, eax
mov eax, 0x4
mov ecx, 0x40
mov edx, 0x6
int 0x80
mov eax, 0x1
mov ebx, 0x0
int 0x80
mov eax, 1
mov ebx, 0
int 0x80

