#include "emulator.h"
#include <iostream>
#include <iterator>

using namespace std;

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

void Emulator::loadProgramInMemory(vector<unsigned char> instructionBytes)
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


vector<Instruction> Emulator::getInstructionVector()
{
	emu_cpu_eip_set(emu_cpu_get(e), static_offset);
	//cout << "Setting EIP to: " << static_offset << endl;
	vector<Instruction> v;
	int startIndex = 0;
	while(emu_cpu_parse(emu_cpu_get(e)) == 0) {

		struct emu_instruction ins = emu_cpu_get(e)->instr;
		bool legalInstruction = false;

		if(ins.cpu.modrm.mod == 3 || emu_cpu_get(e)->cpu_instr_info->format.modrm_byte == 0)
			legalInstruction = true; 

		int endIndex = emu_cpu_eip_get(emu_cpu_get(e)) - static_offset;
		v.push_back(Instruction(ins, legalInstruction, vector<unsigned char>(m_memory.begin()+startIndex, m_memory.begin()+endIndex)));

		//printf("startIndex =  %d, endIndex = %d\n", startIndex, endIndex);

		startIndex = emu_cpu_eip_get(emu_cpu_get(e)) - static_offset;
	}
	
	return v;
}

bool Emulator::getInstruction(vector<unsigned char> insBytes, Instruction &ret)
{


	vector<unsigned char> oldBytes = m_memory;
	loadProgramInMemory(insBytes);

	emu_cpu_eip_set(emu_cpu_get(e), static_offset);
	
	int startIndex = 0;
	
	if(emu_cpu_parse(emu_cpu_get(e)) == 0) {
		struct emu_instruction ins = emu_cpu_get(e)->instr;
		//printf("Opcode %02x\n", ins.cpu.opc);
		bool legalInstruction = false;

		//printf("Prefix: %0d\nOpcode: %02x\n2ndOpcode: %02x\n", ins.cpu.prefixes, ins.cpu.opc, ins.cpu.opc_2nd); 
		//printf("mod = %d\nreg = %d\n", ins.cpu.modrm.mod, ins.cpu.modrm.opc);
		

		if(ins.cpu.modrm.mod == 3 || emu_cpu_get(e)->cpu_instr_info->format.modrm_byte == 0) {
			legalInstruction = true;
		}

		int endIndex = emu_cpu_eip_get(emu_cpu_get(e)) - static_offset;

		Instruction i(ins, legalInstruction, vector<unsigned char>(m_memory.begin()+startIndex, m_memory.begin()+endIndex));

		loadProgramInMemory(oldBytes);

		
		
		ret = i;
		return true;
	} else {
		return false;
	}

	

}

void Emulator::doPreface(vector<unsigned char> program, vector<Instruction> &preface, vector<Instruction> &hep)
{
	loadProgramInMemory(program);
	vector<Instruction> programInstructions = getInstructionVector();

	for(vector<Instruction>::iterator itr = programInstructions.begin(); itr != programInstructions.end(); ++itr) {
		struct emu_instruction ins = itr->getInstruction();
		if(ins.cpu.opc == 141 && ins.cpu.modrm.reg == 5) {
			replaceLEA(*itr, preface, hep);
		} else {
			hep.push_back(*itr);
		}
	}
}

void Emulator::replaceLEA(Instruction ins, vector<Instruction> &preface, vector<Instruction> &hep)
{
	int modMask = 0xC0;
	int regMask = 0x38;
	int rmMask = 0x07;

	// Add lea to preface
	vector<unsigned char> bytes = ins.getBytes();

	int immediate = 0;
	int shifter = 0;

	for(vector<unsigned char>::iterator itr = bytes.begin()+2; itr != bytes.end(); ++itr) {
		immediate += (*itr << shifter);
		shifter += 2;
	}

	bytes.erase(bytes.begin()+2, bytes.end());


	immediate -= 1;
	

	bytes.push_back((immediate & 0x000000FF));
	bytes.push_back((immediate & 0x0000FF00) >> 2);
	bytes.push_back((immediate & 0x00FF0000) >> 4);
	bytes.push_back((immediate & 0xFF000000) >> 6);

	//printf("%02x", (unsigned char)immediate & 0x000000FF);
	//printf("%02x", (unsigned char)(immediate & 0x0000FF00) >> 2);
	//printf("%02x", (unsigned char)(immediate & 0x00FF0000) >> 4);
	//printf("%02x", (unsigned char)(immediate & 0xFF000000) >> 6);

	Instruction newIns;
	getInstruction(bytes, newIns);
	preface.push_back(ins);

	// Add add instruction to hep
	vector<unsigned char> newBytes;
	newBytes.push_back(0x83);
	unsigned char modrm = 0xC0;
	modrm += ins.getInstruction().cpu.modrm.opc;
	newBytes.push_back(modrm);
	newBytes.push_back(0x01);


	getInstruction(newBytes, newIns);
	hep.push_back(newIns);
	
	
}

int Emulator::runAndGetEFlags() 
{
	emu_cpu_eip_set(emu_cpu_get(e), static_offset);
	//cout << "Setting EIP to: " << static_offset << endl;
	emu_cpu_run(emu_cpu_get(e));
	return emu_cpu_eflags_get(emu_cpu_get(e));
}

void Emulator::printDebug()
{
	emu_cpu_debug_print(emu_cpu_get(e));
}