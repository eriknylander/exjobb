#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <iostream>
#include <vector>

class Instruction {


public:
	typedef unsigned char BYTE;
	Instruction(std::vector<BYTE> bytes, int startIndex, int endIndex);
	void printBytes();


private:
	std::vector<BYTE> bytes;
	int startIndex;
	int endIndex;
	BYTE opcode;
	BYTE modregrm;
};

#endif