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

vector<unsigned char> Parser::parseFile(char fileName[], int numberOfInstructions) 
{

  FILE *file = fopen(fileName, "r");

  vector<BYTE> program;

  ud_set_input_file(&ud_obj, file);

  int counter = 0;
    
  while (ud_disassemble(&ud_obj) && counter < numberOfInstructions) {
    const uint8_t *bytes = ud_insn_ptr(&ud_obj);
    program.insert(program.end(), bytes, bytes+ud_insn_len(&ud_obj));
    counter++;
  }

	return program;
}

int Parser::parseUntilInvalid(vector<BYTE> buffer) {
  ud_set_input_buffer(&ud_obj, buffer.data(), buffer.size());

  int index = 0;
  while (ud_disassemble(&ud_obj)) {
    int bytesRead = ud_insn_len(&ud_obj);

    if(strcmp(ud_insn_asm(&ud_obj), "invalid") == 0)
      break;

    //printf("%s\t%s\n", ud_insn_hex(&ud_obj), ud_insn_asm(&ud_obj));

    index += bytesRead;
  }

  return index;

}

void Parser::parseAndPrintPreface(vector<BYTE> preface) {
  printf("PROGRAM!!!\n\nBITS 32\nsection .data\n;ALLOCATE DATA HERE\n");
  printf(";Don't forget data for memory access adjustment\nsection .text\n\tglobal _start\n_start:\n;preface\n");
  printf("\n;Change 0xdeadc0de to 'label - 1'\n"); // Don't know address when doing this. Must use label since the .o-file
                                                   // uses other addresses than the executable file with the elf-header.
  if(preface.size() > 0) {
    ud_set_input_buffer(&ud_obj, preface.data(), preface.size());
    while(ud_disassemble(&ud_obj)) {
      printf("%s\n", ud_insn_asm(&ud_obj));
    }
  }
}

void Parser::parseAndPrintMemoryAdjustment(vector<BYTE> memoryAdjustment) {
  printf("\n;Memory adjustment stuff (Change 0xdeadc0de to according memory access adjusment label)\n");

  if(memoryAdjustment.size() > 0) {
    ud_set_input_buffer(&ud_obj, memoryAdjustment.data(), memoryAdjustment.size());
    while(ud_disassemble(&ud_obj)) {
      printf("%s\n", ud_insn_asm(&ud_obj));
    }
  }
}

void Parser::parseAndPrintProgram(vector<BYTE> mep, int startingOpcodeSize, int index)
{

  printf("\njmp mep%d + 0x%d\n", index, startingOpcodeSize);
  printf("\n;mep\nmep%d:\n", index);

  ud_set_input_buffer(&ud_obj, mep.data(), mep.size());

  while(ud_disassemble(&ud_obj)) {
    printf("%s\n", ud_insn_asm(&ud_obj));
  }
}

void Parser::printProgramReturn() 
{
   printf("mov eax, 1\nmov ebx, 0\nint 0x80\n");
}
