#ifndef CPU_H
#define CPU_H

#include <vector>
#include "instruction.h"

#define EAX 0;
#define ECX 1;
#define EDX 2;
#define EBX 3;
#define ESP 4;
#define EBP 5;
#define ESI 6;
#define EDI 7;

class CPU {
public:
	typedef unsigned int32_t Register_32;
	typedef unsigned int64_t Register_64;
	typedef unsigned int8_t BYTE;

	CPU();
	bool performInstruction();
	bool loadProgram(std::vector<BYTE> program);

	int8_t fetchByteFromMemory(int address);
	int32_t fetchDoubleWordFromMemory(int address);

	bool storeByteInMemory(int8_t value, int address);
	bool storeDoubleWordInMemory(int_32 value, int address);

	int8_t fetchByteFromRegistry(int registry);
	int32_t fetchDoubleWordFromRegistry(int registry);

	bool storeByteInRegistry(int8_t value, int registry);
	bool storeDoubleWordInRegistry(int_32 value, int registry);

	void printRegistryState();
	void printMemoryState();

private:

	//Instruction pointer
	Register_32 eip = 0;

	//Stack pointer
	Register_32 esp = 0;

	//Base pointer
	Register_32 ebp = 0;

	//General registers
	Register_32 eax = 0;
	Register_32 ecx = 0;
	Register_32 edx = 0;
	Register_32 ebx = 0;

	//Index registers
	Register_32 esi = 0;
	Register_32 edi = 0;

	//ELAGS
	Register_64 EFLAGS = 0;

	//Memory
	vector<Instruction> programMemory;
	vector<BYTE> memory;

};





#endif