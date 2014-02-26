#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include "instruction.h"
#include "parser.h"


typedef unsigned char BYTE;

using namespace std;

vector<BYTE> getBytes(vector<Instruction> program) 
{
	vector<BYTE> returnVector;
	for(vector<Instruction>::iterator itr = program.begin(); itr != program.end(); ++itr) {
		itr->getBytes(&returnVector);
	}

	cout << "return size is " << returnVector.size() << endl;

	return returnVector;
}

int main(int argc, char* argv[]) {

	if(argc < 2) {
		cerr << "Error. Usage: program <file path>" << endl;
		return 1;
	}


	Parser p;

	vector<Instruction> program = p.parseBytes(argv[1]);

	vector<BYTE> pureBytes = getBytes(program);	
	
		


	return 0;
	
}