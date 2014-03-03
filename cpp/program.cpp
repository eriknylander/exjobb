#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include "instruction.h"
#include "parser.h"
#include "emulator.h"


typedef unsigned char BYTE;

using namespace std;

/*
vector<BYTE> getBytes(vector<Instruction> program) 
{
	vector<BYTE> returnVector;
	for(vector<Instruction>::iterator itr = program.begin(); itr != program.end(); ++itr) {
		itr->getBytes(&returnVector);
	}
	return returnVector;
}
*/
int main(int argc, char* argv[]) {

	if(argc < 2) {
		cerr << "Error. Usage: program <file path>" << endl;
		return 1;
	}


	Parser p;

	Emulator e;
										
	vector<BYTE> program = p.parseFile(argv[1]);

	//vector<BYTE> program = getBytes(program);


	int validBytes = p.parseUntilInvalid(program.data(), program.size());

	e.loadProgramInMemory(program.data(), program.size());
	vector<struct emu_instruction> instructions = e.getInstructionVector();

	cout << "EFLAGS: " << e.runAndGetEFlags() << endl;
	e.printDebug();	

	
	program.insert(program.begin(), 0x4);

	e.loadProgramInMemory(program.data(), program.size());

	cout << "EFLAGS: " << e.runAndGetEFlags() << endl;
	e.printDebug();

	e.getInstructionVector();
		


	return 0;
	
}