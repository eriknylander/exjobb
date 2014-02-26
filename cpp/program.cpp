#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <iomanip>
#include "instruction.h"
#include "parser.h"
#include "emulator.h"

typedef unsigned char BYTE;

using namespace std;

int main(int argc, char* argv[]) {

	if(argc < 2) {
		cerr << "Error. Usage: program <file path>" << endl;
		return 1;
	}

	ifstream is(argv[1]);
	istream_iterator<BYTE> start(is), end;
	vector<BYTE> bytes(start, end);

	for(int i = 0; i < bytes.size(); i++) {
		cout << (int)bytes.at(i) << endl;
	}

	//BYTE input[] = {0x31, 0xc0, 0x83, 0xc6, 0x01, 0x89, 0x79, 0x08, 0x89, 0x46, 0x0c, 0x90};


	Parser p;
	Emulator e;

	cout << "Unobfuscated shit: " << endl;
	vector<Instruction> program = p.parseBytes(bytes.data(), bytes.size());
	for(int i = 0; i < program.size(); i++) {
		program.at(i).printBytes();
	}

	cout << "EFLAGS = " << e.runAndGetEFlags(bytes.data(), bytes.size()) << endl;

	cout << "Maybe obfuscated shit: " << endl;
	bytes.insert(bytes.begin(), 0x05);
	program = p.parseBytes(bytes.data(), bytes.size());

	for(int i = 0; i < program.size(); i++) {
		program.at(i).printBytes();
	}

	
	cout << "EFLAGS = " << e.runAndGetEFlags(bytes.data(), bytes.size()) << endl;

	return 0;
}