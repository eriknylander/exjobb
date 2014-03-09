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

BYTE opcodes[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
						0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
						0x30, 0x31, 0x32, 0x33, 0x34, 0x35, /*0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 
						0x7c, 0x7d, 0x7e, 0x7f,*/ 0x80, 0x81, 0x82, 0x83, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e};

bool InstructionCompare(const struct emu_cpu_instruction &a, const struct emu_cpu_instruction &b)
{
	return (a.opc == b.opc) && (a.modrm.mod == b.modrm.mod) && (a.modrm.opc == b.modrm.opc) && (a.modrm.sib.base == b.modrm.sib.base)
			&& (a.modrm.sib.index == b.modrm.sib.index) && (a.modrm.sib.scale == a.modrm.sib.scale) && (a.imm == b.imm) 
			&& (a.imm8 == b.imm8) && (a.imm16 == b.imm16) && (a.disp == b.disp);  
}

void printProgram(vector<BYTE> program, BYTE startingByte) 
{
	program.insert(program.begin(), startingByte);
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

	int opcodesSize = sizeof(opcodes) / sizeof(BYTE);
	vector<BYTE> startingBytes(opcodes, opcodes+opcodesSize);


	Parser p;

	Emulator e;
										
	vector<BYTE> program = p.parseFile(argv[1]);



	e.loadProgramInMemory(program.data(), program.size());
	vector<pair<struct emu_instruction, bool> > hep = e.getInstructionVector();

	cout << "Got " << hep.size() << " instructions." << endl;

	
	for(vector<BYTE>::iterator startingBytesIterator = startingBytes.begin(); startingBytesIterator != startingBytes.end(); ++startingBytesIterator) {

		BYTE opcode = *startingBytesIterator;

		printf("%02x\n", opcode);

		int validBytes = p.parseUntilInvalid(program, program.size(), opcode);
		

		e.loadProgramInMemory(program.data(), validBytes, *startingBytesIterator);

		

		vector<pair<struct emu_instruction, bool> > mep = e.getInstructionVector();

		if(mep.size() <= hep.size()) {
			continue;
		}

		printProgram(program, opcode);
		printf("Inserting %02x\n", opcode);
		cout << "MEP is bigger than HEP" << endl;	

		for(vector<pair<struct emu_instruction, bool> >::iterator itr = mep.begin(); itr != mep.end(); ++itr) {
			if(itr->second) 
				cout << "Valid" << endl;
			else
				cout << "Invalid" << endl;
		}

	}

	
	return 0;
	
	
}