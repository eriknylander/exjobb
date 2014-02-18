global main
section .data
        stringlen dd 0xA
        format db "stringlen = %d"

section .text
        
        extern printf
main:
        sub esp, 0x8
        
        mov eax, [format]
        mov [esp], eax
        add esp, 0x4

        mov eax, [stringlen]
        mov [esp], eax


        
        call printf

        
        
