#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <vector>

extern "C" {
#include "emu/emu_cpu_data.h"
#include "emu/emu_instruction.h"
}

class Instruction {


public:
	typedef unsigned char BYTE;
	Instruction(struct emu_instruction input, struct emu_cpu_instruction_info info, bool legalInstruction, std::vector<BYTE> bytes);
	Instruction();

	struct emu_instruction getInstruction();
	struct emu_cpu_instruction_info getInstructionInfo();
	
	bool isLegal();
	int getStartIndex();
	int getEndIndex();
	std::vector<BYTE> getBytes();

	void printInstruction();
	std::string getByteString();

private:
	struct emu_instruction m_ins;
	struct emu_cpu_instruction_info m_info;
	bool m_legalInstruction;
	std::vector<BYTE> m_bytes;


};

#endif