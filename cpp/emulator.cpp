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
	for(int i = 0; i < instructionBytesLen; i++)
	{
		emu_memory_write_byte(mem, static_offset+i, instructionBytes[i]);
	}
}


std::vector<struct emu_instruction> Emulator::getInstructionVector()
{
	emu_cpu_eip_set(emu_cpu_get(e), static_offset);
	std::cout << "Setting EIP to: " << static_offset << std::endl;
	std::vector<struct emu_instruction> v;
	while(emu_cpu_parse(emu_cpu_get(e)) == 0) {
		//std::cout << "Instruction parsed was: " <<  emu_cpu_get(e)->instr_string << std::endl;
		v.push_back(emu_cpu_get(e)->instr);
	}
	
	return v;
}


int Emulator::runAndGetEFlags() 
{
	emu_cpu_eip_set(emu_cpu_get(e), static_offset);
	std::cout << "Setting EIP to: " << static_offset << std::endl;
	emu_cpu_run(emu_cpu_get(e));
	return emu_cpu_eflags_get(emu_cpu_get(e));
}

void Emulator::printDebug()
{
	emu_cpu_debug_print(emu_cpu_get(e));
}