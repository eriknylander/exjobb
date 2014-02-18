#ifndef CPU_H
#define CPU_H

#include<vector>

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
	bool storeDoubleWordInRegistry(int_32 value, int registry);Registry

private:

	//Instruction pointer
	Register_32 EIP;

	//Stack pointer
	Register_32 ESP;

	//Base pointer
	Register_32 EBP;

	//General registers
	Register_32 EAX;
	Register_32 EBX;
	Register_32 ECX;
	Register_32 EDX:

	//Index registers
	Register_32 ESI;
	Register_32 EDI;

	//ELAGS
	Register_64 EFLAGS;

	//Memory
	BYTE programMemory[];
	BYTE memory[];

};





#endif