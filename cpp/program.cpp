#include <iostream>
#include "instruction.h"
#include "parser.h"

typedef unsigned char BYTE;

using namespace std;

int main(int argc, char* argv[]) {

	if(argc < 2) {
		cerr << "Error. Usage: program <file path>" << endl;
		return 1;
	}

	BYTE input[] = {0x31, 0xc0, 0x83, 0xc6, 0x01, 0x89, 0x79, 0x08, 0x89, 0x46, 0x0c, 0x90};


	Parser p;

	vector<Instruction> program = p.parseBytes(argv[1]);


	for(int i = 0; i < program.size(); i++) {
		program.at(i).printBytes();
	}

	return 0;
}