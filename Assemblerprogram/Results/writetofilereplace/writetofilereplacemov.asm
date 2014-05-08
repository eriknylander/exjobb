BITS 32
section .data
;ALLOCATE DATA HERE
;Don't forget data for memory access adjustment
	textoutput db 'Hello', 10
	textoutputlen equ $ - textoutput
	filetoopen db 'hello.txt', 0

	ediData: dd 1
	esiData: dd 1


section .text
	global _start
_start:
;preface

;Change 0xdeadc0de to 'label - 1'

;Memory adjustment stuff (Change 0xdeadc0de to according memory access adjusment label)
lea edi, [ediData]
add edi, 0x47996f01
lea esi, [esiData]
add esi, 0x48

jmp mep0 + 0x3

;mep
mep0:
test dword [edi-0x47996f01], 0xe0c10000
adc [esi-0x48], ah
add eax, 0xbb6600
add cl, al
jecxz 0x28c31de8
mov bx, 0x46
mov cx, 0x0
shl ecx, 0x10
mov cx, 0x2
int 0x80
cmp eax, 0x0
jl 0x28c31dea
mov ebx, eax
mov ax, 0x0
shl eax, 0x10
mov ax, 0x4
mov cx, 0x0
shl ecx, 0x10
mov cx, 0x40
mov dx, 0x0
shl edx, 0x10
mov dx, 0x6
int 0x80
mov ax, 0x0
shl eax, 0x10
mov ax, 0x1
mov bx, 0x0
shl ebx, 0x10
mov bx, 0x0
mov eax, 1
mov ebx, 0
int 0x80
