#include "instruction.h"
#include <iostream>
#include <iomanip>
#include <sstream>


typedef unsigned char BYTE;

/**
* Constructs an Instruction object
* @param ins struct containing characteristics of the instruction, gathered using libemu
* @param info struct containing other characteristics of the instruction, gathered using libemu
* @param legalInstruction shows if the instruction can be in the MEP without crashing it
* @param bytes the bytes making up the instruction
*
*/
Instruction::Instruction(struct emu_instruction ins, struct emu_cpu_instruction_info info, bool legalInstruction, std::vector<BYTE> bytes) : m_ins(ins), 
								m_info(info), m_legalInstruction(legalInstruction), m_bytes(bytes){}

/**
* Constructs an empty Instruction object
*/
Instruction::Instruction() {}

struct emu_instruction Instruction::getInstruction() 
{
	return m_ins;
}

struct emu_cpu_instruction_info Instruction::getInstructionInfo()
{
	return m_info;
}

bool Instruction::isLegal() 
{
	return m_legalInstruction;
}

std::vector<BYTE> Instruction::getBytes() 
{
	return m_bytes;
}

void Instruction::printInstruction() 
{
	for(std::vector<BYTE>::iterator itr = m_bytes.begin(); itr != m_bytes.end(); ++itr) {
			std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)*itr;
	}

	std::cout << std::endl;
}

std::string Instruction::getByteString()
{
	std::stringstream ss;
	for(std::vector<BYTE>::iterator itr = m_bytes.begin(); itr != m_bytes.end(); ++itr) {
			ss << std::hex << std::setfill('0') << std::setw(2) << (int)*itr;
	}

	return ss.str();
}







  