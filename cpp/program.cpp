#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include "instruction.h"
#include "parser.h"
#include "emulator.h"


typedef unsigned char BYTE;

using namespace std;

vector<BYTE> getBytes(vector<Instruction> program) 
{
	vector<BYTE> returnVector;
	for(vector<Instruction>::iterator itr = program.begin(); itr != program.end(); ++itr) {
		itr->getBytes(&returnVector);
	}
	return returnVector;
}

int main(int argc, char* argv[]) {

	if(argc < 2) {
		cerr << "Error. Usage: program <file path>" << endl;
		return 1;
	}


	Parser p;

	Emulator e;

	vector<Instruction> program = p.parseFile(argv[1]);

	vector<BYTE> pureBytes = getBytes(program);


	int validBytes = p.parseUntilInvalid(pureBytes.data(), pureBytes.size());

	cout << "EFLAGS: " << e.runAndGetEFlags(pureBytes.data(), pureBytes.size()) << endl;	

	pureBytes.insert(pureBytes.begin(), 0x03);

	validBytes = p.parseUntilInvalid(pureBytes.data(), pureBytes.size());

	cout << "EFLAGS: " << e.runAndGetEFlags(pureBytes.data(), pureBytes.size()) << endl; 
	
		


	return 0;
	
}