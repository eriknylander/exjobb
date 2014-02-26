#include <stdio.h>
#include <string.h>
#include <udis86.h>




void generateAssembler(char* bytes, int len) 
{

    ud_t ud_obj;

    ud_init(&ud_obj);

   
    ud_set_input_buffer(&ud_obj, bytes, len);
    ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);
    ud_set_syntax(&ud_obj, UD_SYN_INTEL);
    ud_set_mode(&ud_obj, 32);

    while (ud_disassemble(&ud_obj)) {
        printf("%s\n", ud_insn_asm(&ud_obj));
    }
}

int calcSpaces(char* source) 
{
    int spaceCount = 0;
    
    char *c = source;
    char *end = source + strlen(source);
        while(c != end) {
        if(*c == ' ') {
            spaceCount++;
        }
        c++;
    }

    return spaceCount;
}

void getInputToDisassemble(char* source, char* destination, int destLen) 
{
    char* end = destination + destLen;
    unsigned int u;

    while(destination < end && sscanf(source, "%2x", &u) == 1) {
        *destination++ = u;
        source += 2;
    }
}

void removeSpaces(char* source, char* destination) 
{
    char *c = source;
    char *end = source + strlen(source);
        while(c != end) {
        if(*c != ' ') {
            *destination++ = *c;
        }
        c++;
    }
}




int main(int argc, char* argv[])
{
    if(argc < 2) {
        return 1;
    }

    int i = 1;
    while(i < argc) {
        char fixedInputFromArg[strlen(argv[i]) - calcSpaces(argv[i])];
        fixedInputFromArg[strlen(argv[i]) - calcSpaces(argv[i])] = '\0';
        removeSpaces(argv[i], fixedInputFromArg);

        int inputLen = strlen(fixedInputFromArg) / 2;

        char inputToDisassemble[inputLen];

        getInputToDisassemble(fixedInputFromArg, inputToDisassemble, inputLen);

        generateAssembler(inputToDisassemble, inputLen);
        i++;  

    }  
    

    
    return 0;

}   
