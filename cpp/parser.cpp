/**
* parser.cpp
*/



#include "parser.h"
#include "instruction.h"
#include <udis86.h>
#include <iterator>
#include <stdio.h>
#include <string.h>


using namespace std;


/*
* Creates a parser object and sets up the Udis86 object for parsing and disassembly
*/
Parser::Parser() 
{
   ud_init(&ud_obj);
   ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);    
   ud_set_syntax(&ud_obj, UD_SYN_INTEL);
   ud_set_mode(&ud_obj, 32);
}

/**
* Reads an input file using Udis86 and places the bytes in a vector
* 
* @param fileName  the path of the input file
* @param numberOfInstructions the number of instructions to read from the input file
* @return a vector containing the bytes making up the input file
*/
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


/**
* Disassembles a vector of bytes and returns the number of valid instructions
* @param buffer the vector containing the bytes
* @return the number of valid instructions
*/
int Parser::parseUntilInvalid(vector<BYTE> buffer) {
  ud_set_input_buffer(&ud_obj, buffer.data(), buffer.size());

  int index = 0;
  while (ud_disassemble(&ud_obj)) {
    int bytesRead = ud_insn_len(&ud_obj);

    string assembler = ud_insn_asm(&ud_obj);
    size_t found = assembler.find("invalid");

    if(found != string::npos)
      break;

    //printf("%s\t%s\n", ud_insn_hex(&ud_obj), ud_insn_asm(&ud_obj));

    index += bytesRead;
  }

  return index;

}

/**
* Disassembles a vector of bytes and prints the instructions in assembly language
* @param preface the vector containing the bytes in the preface
*/

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

/**
* Disassembles a vector of bytes and prints the instructions in assembly language
* @param memoryAdjustment the vector containing the bytes in the memory adjustment part
*/
void Parser::parseAndPrintMemoryAdjustment(vector<BYTE> memoryAdjustment) {
  printf("\n;Memory adjustment stuff (Change 0xdeadc0de to according memory access adjusment label)\n");

  if(memoryAdjustment.size() > 0) {
    ud_set_input_buffer(&ud_obj, memoryAdjustment.data(), memoryAdjustment.size());
    while(ud_disassemble(&ud_obj)) {
      printf("%s\n", ud_insn_asm(&ud_obj));
    }
  }
}

/**
* Disassembles a vector of bytes and prints the instructions in assembly language
* @param mep the vector containing the bytes in the program
* @param startingOpcodeSize the number of starting bytes
* @param index an index for the mep
*/
void Parser::parseAndPrintProgram(vector<BYTE> mep, int startingOpcodeSize, int index)
{

  printf("\njmp mep%d + 0x%d\n", index, startingOpcodeSize);
  printf("\n;mep\nmep%d:\n", index);

  ud_set_input_buffer(&ud_obj, mep.data(), mep.size());

  while(ud_disassemble(&ud_obj)) {
    printf("%s\n", ud_insn_asm(&ud_obj));
  }
}

/**
* Prints a return statement in assembly language
*/
void Parser::printProgramReturn() 
{
   printf("mov eax, 1\nmov ebx, 0\nint 0x80\n");
}
