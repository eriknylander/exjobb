#include "emulator.h"


Emulator::Emulator() 
{
	e = emu_new();
	mem = emu_memory_get(e);
}

Emulator::~Emulator() 
{
	emu_free(e);
}


int Emulator::runAndGetEFlags(unsigned char instructionBytes[], int instructionBytesLen) 
{

	int static_offset = 4711;

	int i;
	for(i = 0; i < instructionBytesLen; i++)
	{
		emu_memory_write_byte(mem, static_offset+i, instructionBytes[i]);
	}
	emu_memory_write_byte(mem, static_offset+i, '\xcc');

	emu_cpu_eip_set(emu_cpu_get(e), static_offset);
	emu_cpu_run(emu_cpu_get(e));

	return emu_cpu_eflags_get(emu_cpu_get(e));

}
