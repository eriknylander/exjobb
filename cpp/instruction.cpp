#include "instruction.h"
#include <iostream>
#include <iomanip>
#include <iterator>



Instruction::Instruction(std::vector<BYTE> bytes, int startIndex, int endIndex) : bytes(bytes), startIndex(startIndex), endIndex(endIndex) 
{
	opcode = bytes[0];
	if(sizeof(bytes) > 1)
		modregrm = bytes[1];
}

void Instruction::printBytes() 
{
	for(std::vector<BYTE>::iterator itr = bytes.begin(); itr != bytes.end(); ++itr) {
		std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)*itr;
	}

	std::cout << std::endl;

	//std::cout << "opcode: " << std::hex << std::setfill('0') << std::setw(2) << (int)opcode << std::endl;
	//std::cout << "modregrm: " << std::hex << std::setfill('0') << std::setw(2) << (int)modregrm << std::endl;
}


void Instruction::getBytes(std::vector<unsigned char> *acc) 
{
	for(int i = 0; i < bytes.size(); i++) {
		acc->push_back(bytes.at(i));
	}
}


  