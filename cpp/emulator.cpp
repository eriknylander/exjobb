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

void Emulator::loadProgramInMemory(unsigned char instructionBytes[], int instructionBytesLen)
{
	int i;
	for(i = 0; i < instructionBytesLen; i++)
	{
		emu_memory_write_byte(mem, static_offset+i, instructionBytes[i]);
	}
	emu_memory_write_byte(mem, static_offset+i, 0xcc);
}


std::vector<std::pair<struct emu_instruction, bool> > Emulator::getInstructionVector()
{
	emu_cpu_eip_set(emu_cpu_get(e), static_offset);
	//std::cout << "Setting EIP to: " << static_offset << std::endl;
	std::vector<std::pair<struct emu_instruction, bool> > v;
	while(emu_cpu_parse(emu_cpu_get(e)) == 0) {

		/*if(emu_cpu_get(e)->cpu_instr_info->format.modrm_byte == 0) {
			std::cout << "Does not have modrm" << std::endl;
		} else {
			std::cout << "Has modrm" << std::endl;
		}*/

		v.push_back(std::make_pair(emu_cpu_get(e)->instr, emu_cpu_get(e)->cpu_instr_info->format.modrm_byte == 0));
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