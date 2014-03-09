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

void Emulator::loadProgramInMemory(unsigned char instructionBytes[], int instructionBytesLen, unsigned char startingByte)
{
	emu_memory_write_byte(mem, static_offset, startingByte);
	int i;
	for(i = 1; i < instructionBytesLen; i++)
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
		struct emu_instruction ins = emu_cpu_get(e)->instr;
		bool validInstruction = false;

		if(ins.cpu.modrm.mod == 3 || emu_cpu_get(e)->cpu_instr_info->format.modrm_byte == 0) {
			validInstruction = true;
			//std::cout << "Instruction valid: Instruction has valid mod bits or no modrm" << std::endl;
		}


		v.push_back(std::make_pair(ins, validInstruction));
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