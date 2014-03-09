#include "instruction.h"




Instruction::Instruction(struct emu_instruction ins, bool legalInstruction, int startIndex, int endIndex) : m_ins(ins), m_legalInstruction(legalInstruction), m_startIndex(startIndex), m_endIndex(endIndex) {}

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







  