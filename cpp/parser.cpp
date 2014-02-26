#include "parser.h"
#include "instruction.h"
#include <udis86.h>
#include <iterator>
#include <stdio.h>
#include <string.h>


using namespace std;

Parser::Parser() 
{
   ud_init(&ud_obj);
   ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);    
   ud_set_syntax(&ud_obj, UD_SYN_INTEL);
   ud_set_mode(&ud_obj, 32);
}

vector<Instruction> Parser::parseFile(char fileName[]) 
{

  FILE *file = fopen(fileName, "r");

  vector<Instruction> program;

   
  ud_set_input_file(&ud_obj, file);
    
  int index = 0;
  while (ud_disassemble(&ud_obj)) {

    vector<BYTE> instructionBytes;
    int bytesRead = ud_insn_len(&ud_obj);

    const uint8_t* bytes = ud_insn_ptr(&ud_obj);

    for(int i = 0; i < bytesRead; i++) {
    	instructionBytes.push_back((*(bytes+i)));
    }



    Instruction ins(instructionBytes, index, index+bytesRead);

    program.push_back(ins);
    index += index + bytesRead;
  }



	return program;
}

int Parser::parseUntilInvalid(BYTE input[], int len) {
  ud_set_input_buffer(&ud_obj, input, len);

  int index = 0;
  while (ud_disassemble(&ud_obj)) {
    int bytesRead = ud_insn_len(&ud_obj);

    if(strcmp(ud_insn_asm(&ud_obj), "invalid") == 0)
      break;

    printf("%s\t%s\n", ud_insn_hex(&ud_obj), ud_insn_asm(&ud_obj));

    index += bytesRead;
  }

  return index;

}
