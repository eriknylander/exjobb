#include "emulator.h"
#include <iostream>

Emulator::Emulator() 
{
	e = emu_new();
	mem = emu_memory_get(e);
	emu_log_level_set(emu_logging_get(e), EMU_LOG_DEBUG);
	static_offset = 4711;
}

Emulator::~Emulator() 
{
	emu_free(e);
}

void Emulator::loadProgramInMemory(std::vector<unsigned char> instructionBytes)
{
	m_memory = instructionBytes;
	int instructionBytesLen = instructionBytes.size();
	int i;
	for(i = 0; i < instructionBytesLen; i++)
	{
		emu_memory_write_byte(mem, static_offset+i, instructionBytes.at(i));
	}
	emu_memory_write_byte(mem, static_offset+i, 0xcc);
}


std::vector<Instruction> Emulator::getInstructionVector()
{
	emu_cpu_eip_set(emu_cpu_get(e), static_offset);
	//std::cout << "Setting EIP to: " << static_offset << std::endl;
	std::vector<Instruction> v;
	int startIndex = 0;
	while(emu_cpu_parse(emu_cpu_get(e)) == 0) {

		struct emu_instruction ins = emu_cpu_get(e)->instr;
		bool legalInstruction = false;

		if(ins.cpu.modrm.mod == 3 || emu_cpu_get(e)->cpu_instr_info->format.modrm_byte == 0)
			legalInstruction = true; 

		int endIndex = emu_cpu_eip_get(emu_cpu_get(e)) - static_offset - 1;
		v.push_back(Instruction(ins, legalInstruction, startIndex, endIndex, std::vector<unsigned char>(m_memory.begin()+startIndex, m_memory.begin()+endIndex)));

		//printf("startIndex =  %d, endIndex = %d\n", startIndex, endIndex);

		startIndex = emu_cpu_eip_get(emu_cpu_get(e)) - static_offset;
	}
	
	return v;
}


int Emulator::runAndGetEFlags() 
{
	emu_cpu_eip_set(emu_cpu_get(e), static_offset);
	//std::cout << "Setting EIP to: " << static_offset << std::endl;
	emu_cpu_run(emu_cpu_get(e));
	return emu_cpu_eflags_get(emu_cpu_get(e));
}

void Emulator::printDebug()
{
	emu_cpu_debug_print(emu_cpu_get(e));
}