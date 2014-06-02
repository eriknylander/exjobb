BITS 32

tohide:
mov ax, 0x15
mov bx, 0x1
and eax, ebx

mep:
nop word [esi-0x56ffea48]
