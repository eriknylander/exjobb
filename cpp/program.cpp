#include <iostream>
#include <iomanip>
#include <utility>
#include <fstream>
#include <vector>
#include <iterator>
#include "instruction.h"
#include "parser.h"
#include "emulator.h"


typedef unsigned char BYTE;

using namespace std;

/*BYTE opcodes[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
						0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
						0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 
						0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e};*/

int prefixMask = 0xFF0000;
int firstOpcodeMask = 0x00FF00;
int secondOpcodeMask = 0x0000FF;

struct Opcode 
{
	Opcode(int opcode) 
	{
		if(opcode > 0x00FFFF) {
			bytes.push_back(opcode & secondOpcodeMask);
			bytes.push_back((opcode & firstOpcodeMask) >> 2);
			bytes.push_back((opcode & prefixMask) >> 4);
		} else if(opcode > 0x0000FF) {
			bytes.push_back(opcode & secondOpcodeMask);
			bytes.push_back((opcode & firstOpcodeMask) >> 2);
		} else {
			bytes.push_back(opcode & secondOpcodeMask);
		}
	}

	vector<BYTE> bytes;
};


bool InstructionCompare(const struct emu_cpu_instruction &a, const struct emu_cpu_instruction &b)
{
	return (a.opc == b.opc) && (a.modrm.mod == b.modrm.mod) && (a.modrm.opc == b.modrm.opc) && (a.modrm.sib.base == b.modrm.sib.base)
			&& (a.modrm.sib.index == b.modrm.sib.index) && (a.modrm.sib.scale == a.modrm.sib.scale) && (a.imm == b.imm) 
			&& (a.imm8 == b.imm8) && (a.imm16 == b.imm16) && (a.disp == b.disp);  
}

void printProgram(vector<BYTE> program) 
{
	for(vector<BYTE>::iterator itr = program.begin(); itr != program.end(); ++itr) {
		cout << hex << setfill('0') << setw(2) << (int)*itr << " ";
	}

	cout << endl;
}


int main(int argc, char* argv[]) {

	if(argc < 2) {
		cerr << "Error. Usage: program <file path>" << endl;
		return 1;
	}

	vector<pair<BYTE, int> > startingBytes;

	for(BYTE i = 0; i < 255; i++) {
		startingBytes.push_back(make_pair(i, -1));
	}

	startingBytes.push_back(make_pair(255, -1));


	Parser p;

	Emulator e;
										
	vector<BYTE> program = p.parseFile(argv[1]);



	e.loadProgramInMemory(program.data(), program.size());
	vector<Instruction> hep = e.getInstructionVector();

	cout << "Got " << hep.size() << " instructions." << endl;

	int validBytesHep = p.parseUntilInvalid(program.data(), program.size());

	
	/*for(vector<struct emu_instruction>::iterator itr = hep.begin(); itr != hep.end(); ++itr) {
		printf("Opcode = %02x , Mod = %02x , Reg = %02x , Rm = %02x\n", itr->opc, itr->cpu.modrm.mod, itr->cpu.modrm.opc, itr->cpu.modrm.rm);	
	} */
	
	for(vector<pair<BYTE, int> >::iterator startingBytesIterator = startingBytes.begin(); startingBytesIterator != startingBytes.end(); ++startingBytesIterator) {
		
		program.insert(program.begin(), startingBytesIterator->first);

		BYTE opcode = startingBytesIterator->first;

		int validBytesMep = p.parseUntilInvalid(program.data(), program.size());

		e.loadProgramInMemory(program.data(), validBytesMep);

		vector<Instruction> mep = e.getInstructionVector();

		if(validBytesMep <= validBytesHep) {
			program.erase(program.begin());
			continue;
		}

		//printf("Inserting %02x\n", opcode);
		//cout << "MEP is bigger than HEP" << endl;

		//printProgram(program);	

		for(vector<Instruction>::iterator itr = mep.begin(); itr != mep.end(); ++itr) {
			if(itr->isLegal()) {
				//cout << "Legal instruction" << endl;
				//cout << "My position is: " <<  dec << itr->getStartIndex() << " - " << itr->getEndIndex() << endl;
			}
			else {
				//cout << "Illegal instruction" << endl;
				//cout << "My position is: " << dec << itr->getStartIndex() << " - " << itr->getEndIndex() << endl;
				startingBytesIterator->second = distance(mep.begin(), itr);
				break;
			}

		}	
		
		program.erase(program.begin());

		cout << "Byte " << hex << setfill('0') << setw(2) << (int)startingBytesIterator->first << " generated " << dec 
		<< 	startingBytesIterator->second <<  " legal instructions." <<endl;
	}

	
	return 0;
	
	
}