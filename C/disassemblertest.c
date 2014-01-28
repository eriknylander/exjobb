#include <stdio.h>
#include <udis86.h>

int main(int argc, char* argv[])
{
    ud_t ud_obj;

    ud_init(&ud_obj);

    FILE* f = fopen(argv[1], "r");
    ud_set_input_file(&ud_obj, f);
    ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);
    ud_set_syntax(&ud_obj, UD_SYN_INTEL);
    ud_set_mode(&ud_obj, 32);

    while (ud_disassemble(&ud_obj)) {
        printf("\t%s\n", ud_insn_asm(&ud_obj));
    }
    
    

    return 0;
}