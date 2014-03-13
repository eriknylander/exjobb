#include "instruction.h"
#include <iostream>
#include <iomanip>


typedef unsigned char BYTE;

Instruction::Instruction(struct emu_instruction ins, bool legalInstruction, int startIndex, int endIndex, std::vector<BYTE> bytes) : m_ins(ins), 
								m_legalInstruction(legalInstruction), m_startIndex(startIndex), m_endIndex(endIndex), m_bytes(bytes){}

struct emu_instruction Instruction::getInstruction() 
{
	return m_ins;
}

bool Instruction::isLegal() 
{
	return m_legalInstruction;
}

int Instruction::getStartIndex()
{
	return m_startIndex;
}

int Instruction::getEndIndex() 
{
	return m_endIndex;
}

std::vector<BYTE> Instruction::getBytes() 
{
	return m_bytes;
}

void Instruction::printInstruction() 
{
	for(std::vector<BYTE>::reverse_iterator itr = m_bytes.rbegin(); itr != m_bytes.rend(); ++itr) {
			std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)*itr;
			//ss << bytes.size() << " bytes : " << (int)*itr;
	}

	std::cout << std::endl;
}







  