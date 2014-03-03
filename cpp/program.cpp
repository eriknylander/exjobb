#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include "instruction.h"
#include "parser.h"
#include "emulator.h"


typedef unsigned char BYTE;

using namespace std;

BYTE startingBytes[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
						0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
						0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 
						0x7c, 0x7d, 0x7e, 0x7f,  0x80, 0x81, 0x82, 0x83, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e};

int main(int argc, char* argv[]) {

	if(argc < 2) {
		cerr << "Error. Usage: program <file path>" << endl;
		return 1;
	}


	Parser p;

	Emulator e;
										
	vector<BYTE> program = p.parseFile(argv[1]);

	program.push_back(0xcc);


	e.loadProgramInMemory(program.data(), program.size());
	vector<struct emu_instruction> instructions = e.getInstructionVector();

	cout << "Got " << instructions.size() << " instructions." << endl;

	
	for(vector<struct emu_instruction>::iterator itr = instructions.begin(); itr != instructions.end(); ++itr) {
		printf("Opcode = %02x , Mod = %02x , Reg = %02x , Rm = %02x\n", itr->opc, itr->cpu.modrm.mod, itr->cpu.modrm.opc, itr->cpu.modrm.rm);	
	}
	

	cout << "EFLAGS: " << e.runAndGetEFlags() << endl;
	//e.printDebug();	

	return 0;
	
}