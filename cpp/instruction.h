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
	Instruction(struct emu_instruction input, bool legalInstruction, int startIndex, int endIndex);

	struct emu_instruction getInstruction();
	bool isLegal();
	int getStartIndex();
	int getEndIndex();

private:
	struct emu_instruction m_ins;
	bool m_legalInstruction;
	int m_startIndex;
	int m_endIndex;
	
};

#endif