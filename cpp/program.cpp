#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include "instruction.h"
#include "parser.h"


typedef unsigned char BYTE;

using namespace std;


int main(int argc, char* argv[]) {

	if(argc < 2) {
		cerr << "Error. Usage: program <file path>" << endl;
		return 1;
	}


	Parser p;

	vector<Instruction> program = p.parseBytes(argv[1]);

	
	

	return 0;
}