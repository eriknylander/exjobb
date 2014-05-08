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

	for(int i = 0; i < 8; i++) {
		usedRegs[i] = 0;
	}
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
		struct emu_cpu_instruction_info info = *(emu_cpu_get(e)->cpu_instr_info);
		bool legalInstruction = false;

		if(ins.cpu.modrm.mod == 3 || emu_cpu_get(e)->cpu_instr_info->format.modrm_byte == 0)
			legalInstruction = true; 

		int endIndex = emu_cpu_eip_get(emu_cpu_get(e)) - static_offset;
		v.push_back(Instruction(ins, info, legalInstruction, vector<unsigned char>(m_memory.begin()+startIndex, m_memory.begin()+endIndex)));

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
		struct emu_cpu_instruction_info info = *(emu_cpu_get(e)->cpu_instr_info);
		//printf("Opcode %02x\n", ins.cpu.opc);
		bool legalInstruction = false;

		//printf("Prefix: %0d\nOpcode: %02x\n2ndOpcode: %02x\n", ins.cpu.prefixes, ins.cpu.opc, ins.cpu.opc_2nd); 
		//printf("mod = %d\nreg = %d\n", ins.cpu.modrm.mod, ins.cpu.modrm.opc);
		

		if(ins.cpu.modrm.mod == 3 || emu_cpu_get(e)->cpu_instr_info->format.modrm_byte == 0) {
			legalInstruction = true;
		}

		int endIndex = emu_cpu_eip_get(emu_cpu_get(e)) - static_offset;

		Instruction i(ins, info, legalInstruction, vector<unsigned char>(m_memory.begin()+startIndex, m_memory.begin()+endIndex));

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

// unsigned char swapNumbers(unsigned char byte) 
// {
// 	unsigned char mostSignificant = byte & 0xF0;
// 	unsigned char leastSignificant = byte & 0x0F;

// 	mostSignificant >>= 4;
// 	leastSignificant <<=4;

// 	printf("mostSignificant = %02x, leastSignificant = %02x\n", mostSignificant, leastSignificant);


// 	return mostSignificant + leastSignificant;
// }

void Emulator::replaceLEA(Instruction ins, vector<Instruction> &preface, vector<Instruction> &hep)
{
	int modMask = 0xC0;
	int regMask = 0x38;
	int rmMask = 0x07;

	// Add lea to preface
	vector<unsigned char> bytes = ins.getBytes();

	// Make note that this register has been adjusted for
	usedRegs[ins.getInstruction().cpu.modrm.opc]++;

	unsigned int immediate = 0;
	int shifter = 3*8;

	for(vector<unsigned char>::reverse_iterator itr = bytes.rbegin(); itr != bytes.rend()-2; ++itr) {
		
		unsigned char byte = *itr/*swapNumbers(*itr)*/;
		
		int buffer = byte;
		buffer <<= shifter;
		
		immediate += buffer;

		shifter -= 8;
	}

	bytes.erase(bytes.begin()+2, bytes.end());

	immediate -= 1;
	
	int b1 = /*immediate & 0x000000ff*/0xde;
	int b2 = /*(immediate & 0x0000ff00) >> 8*/0xc0;
	int b3 = /*(immediate & 0x00ff0000) >> 16*/0xad;
	int b4 = /*(immediate & 0xff000000) >> 24*/0xde;

	

	bytes.push_back((unsigned char)b1);
	bytes.push_back((unsigned char)b2);
	bytes.push_back((unsigned char)b3);
	bytes.push_back((unsigned char)b4);



	Instruction newIns;
	getInstruction(bytes, newIns);
	preface.push_back(newIns);

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

void Emulator::adjustForMemoryAccess(vector<Instruction> &memoryAdjustment, vector<Instruction> &mep)
{
	

	bool pushedESP = false;
	bool pushedEBP = false;

	vector<unsigned char> newBytes;
	Instruction newIns; 

	for(vector<Instruction>::iterator itr = mep.begin(); itr != mep.end(); ++itr) {
		
		struct emu_instruction ins = itr->getInstruction();
		struct emu_cpu_instruction_info info = itr->getInstructionInfo();

		//printf("%x\n", ins.cpu.modrm.ea);
		if(ins.cpu.modrm.mod < 3 && info.format.modrm_byte != 0) {

			newBytes.clear();

			if(usedRegs[ins.cpu.modrm.reg] != 0)
				continue;

			usedRegs[ins.cpu.modrm.reg] = 1;	

			if(ins.cpu.modrm.reg == 0x4) {
				newBytes.push_back(0x54);
				getInstruction(newBytes, newIns);
				memoryAdjustment.push_back(newIns);
				pushedESP = true;
				newBytes.clear();

			} else if (ins.cpu.modrm.reg == 0x5) {
				newBytes.push_back(0x55);
				getInstruction(newBytes, newIns);
				memoryAdjustment.push_back(newIns);
				pushedEBP = true;
				newBytes.clear();
			}

			
			newBytes.push_back(0x8d);
			unsigned char mod = 0;
			unsigned char reg = ins.cpu.modrm.reg;
			reg <<= 3;
			unsigned char rm = 5;
			mod += reg;
			mod += rm;

			newBytes.push_back(mod);
			newBytes.push_back(0xde);
			newBytes.push_back(0xc0);
			newBytes.push_back(0xad);
			newBytes.push_back(0xde);

			
			getInstruction(newBytes,newIns);
			memoryAdjustment.push_back(newIns);

			newBytes.clear();
			mod = 0;
			newBytes.push_back(0x81);
			signed int ea = ins.cpu.modrm.ea;
			//printf("ea = %d\n", ea);
			if(ea < 0) {
				mod = 0xc0;
				mod += ins.cpu.modrm.reg;
				newBytes.push_back(mod);
				//printf("mod = %02x\n", mod);
				ea = ~ea; // 2-complement coversion
				ea++;
			} else {
				mod = 0xe0;
				mod += (ins.cpu.modrm.reg+8);
				newBytes.push_back(mod);
				//printf("mod = %02x\n", mod);
			}

			

			newBytes.push_back((unsigned char)ea);
			newBytes.push_back((unsigned char)(ea >> 8));
			newBytes.push_back((unsigned char)(ea >> 16));
			newBytes.push_back((unsigned char)(ea >> 24));

			getInstruction(newBytes, newIns);

			memoryAdjustment.push_back(newIns);


		}
	}

	if(pushedESP) {
		newBytes.clear();

		newBytes.push_back(0x5c);
		getInstruction(newBytes, newIns);
		mep.push_back(newIns);
	}

	if(pushedEBP) {
		newBytes.clear();
		newBytes.push_back(0x5d);
		getInstruction(newBytes, newIns);
		mep.push_back(newIns);
		printf("Popping ebp\n");
	}


}

vector<Instruction> Emulator::substituteMov(Instruction &a) {
	vector<Instruction> ret;

	emu_instruction ins = a.getInstruction();
	emu_cpu_instruction_info info = a.getInstructionInfo();

	unsigned char reg = ins.cpu.opc - 0xb8;

	vector<unsigned char> newBytes;
	Instruction newIns;

	// Insert 16-bit mov
	newBytes.push_back(0x66);
	newBytes.push_back(ins.cpu.opc);
	
	vector<unsigned char> aBytes = a.getBytes();
	unsigned char a4 = aBytes.back();
	aBytes.pop_back();
	unsigned char a3 = aBytes.back();
	aBytes.pop_back();
	unsigned char a2 = aBytes.back();
	aBytes.pop_back();
	unsigned char a1 = aBytes.back();
	aBytes.pop_back();

	newBytes.insert(newBytes.begin()+2, a3);
	newBytes.insert(newBytes.begin()+3, a4);

	getInstruction(newBytes, newIns);
	//newIns.printInstruction();

	ret.push_back(newIns);

	newBytes.clear();


	// Shift left
	newBytes.push_back(0xc1);
	unsigned char modrm = 0xe0 + reg;
	newBytes.push_back(modrm);
	newBytes.push_back(0x10);

	getInstruction(newBytes, newIns);
	//newIns.printInstruction();


	ret.push_back(newIns);
	newBytes.clear();

	// 16-bit move again for the rest of the bytes
	newBytes.push_back(0x66);
	newBytes.push_back(ins.cpu.opc);
	
	newBytes.insert(newBytes.begin()+2, a1);
	newBytes.insert(newBytes.begin()+3, a2);

	getInstruction(newBytes, newIns);
	//newIns.printInstruction();

	ret.push_back(newIns);

	return ret;

}

vector<Instruction> Emulator::optimizeHep(vector<Instruction> hep) {
	vector<Instruction> retVector;

	for(vector<Instruction>::iterator itr = hep.begin(); itr != hep.end(); ++itr) {
		emu_instruction ins = itr->getInstruction();
		emu_cpu_instruction_info info = itr->getInstructionInfo();
		//printf("Instruction with opc_2nd: %02x\n", ins.cpu.opc_2nd);
		if(ins.cpu.opc >= 0xb8 && ins.cpu.opc <= 0xbf && ins.cpu.prefixes != 0x66) {
			//Substitute long mov instruction
			vector<Instruction> newInstructions = substituteMov(*itr);
			retVector.insert(retVector.end(), newInstructions.begin(), newInstructions.end());
		} else {
			retVector.push_back(*itr);
		}

		
	}

	return retVector;
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