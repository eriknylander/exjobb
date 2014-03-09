BITS 32
global _start

_start:
mov ecx, 0xf
xor eax, eax
test eax, eax
cmove eax, ecx
