BITS 32

jmp l1
db 0x8d

l1:
xor eax, 0x12345678
int 0x80
