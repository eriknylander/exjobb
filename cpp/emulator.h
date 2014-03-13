#ifndef EMULATOR_H
#define EMULATOR_H 

#include <vector>
#include <utility>
#include "instruction.h"

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

	void loadProgramInMemory(std::vector<unsigned char> instructionBytes);
	std::vector<Instruction> getInstructionVector();
	int runAndGetEFlags();
	void printDebug();

private:
	struct emu *e;
	struct emu_memory *mem;
	int static_offset;
	std::vector<unsigned char> m_memory;
};

#endif