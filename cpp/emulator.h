#ifndef EMULATOR_H
#define EMULATOR_H 

extern "C" {
#include "/home/erik/libemu/libemu/include/emu/emu.h"
#include "/home/erik/libemu/libemu/include/emu/emu_memory.h"
#include "/home/erik/libemu/libemu/include/emu/emu_cpu.h"
#include "/home/erik/libemu/libemu/include/emu/emu_log.h"
}


class Emulator {
public:
	Emulator();
	~Emulator();
	int runAndGetEFlags(unsigned char instructionBytes[], int instructionBytesLen);

private:
	struct emu *e;
	struct emu_memory *mem;


};





#endif