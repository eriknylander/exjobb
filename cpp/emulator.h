#ifndef EMULATOR_H
#define EMULATOR_H 

#include <vector>
#include <utility>

extern "C" {
#include "emu/emu.h"
#include "emu/emu_memory.h"
#include "emu/emu_cpu.h"
#include "emu/emu_log.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_instruction.h"
}

class Emulator {


public:
	Emulator();
	~Emulator();

	void loadProgramInMemory(unsigned char instructionBytes[], int instructionBytesLen);
	void loadProgramInMemory(unsigned char instructionBytes[], int instructionBytesLen, unsigned char startingByte);
	std::vector<std::pair<struct emu_instruction, bool> > getInstructionVector();
	int runAndGetEFlags();
	void printDebug();

private:
	struct emu *e;
	struct emu_memory *mem;
	int static_offset;
};

#endif