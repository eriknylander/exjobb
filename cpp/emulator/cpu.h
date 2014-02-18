#ifndef CPU_H
#define CPU_H

#include <vector>

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
	void performInstruction();
	bool loadProgram(std::vector<BYTE> program);

	int8_t fetchByteFromMemory(int address);
	int32_t fetchDoubleWordFromMemory(int address);

	bool storeByteInMemory(int8_t value, int address);
	bool storeDoubleWordInMemory(int_32 value, int address);

	int8_t fetchByteFromRegistry(int registry);
	int32_t fetchDoubleWordFromRegistry(int registry);

	bool storeByteInRegistry(int8_t value, int registry);
	bool storeDoubleWordInRegistry(int_32 value, int registry);

private:

	//Instruction pointer
	Register_32 eip;

	//Stack pointer
	Register_32 esp;

	//Base pointer
	Register_32 ebp;

	//General registers
	Register_32 eax;
	Register_32 ecx;
	Register_32 edx;
	Register_32 ebx:

	//Index registers
	Register_32 esi;
	Register_32 edi;

	//ELAGS
	Register_64 EFLAGS;

	//Memory
	vector<BYTE> programMemory;
	vector<BYTE> memory;

};





#endif