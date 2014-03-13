#include <iostream>
#include <iomanip>
#include <utility>
#include <fstream>
#include <vector>
#include <iterator>
#include <sstream>
#include "instruction.h"
#include "parser.h"
#include "emulator.h"


typedef unsigned char BYTE;

using namespace std;	

int prefixMask = 0xFF0000;
int firstOpcodeMask = 0x00FF00;
int secondOpcodeMask = 0x0000FF;

struct Opcode 
{
	vector<BYTE> bytes;
	string getOpcodeString() {
		stringstream ss;
		for(vector<BYTE>::reverse_iterator itr = bytes.rbegin(); itr != bytes.rend(); ++itr) {
			ss << hex << setfill('0') << setw(2) << (int)*itr;
			//ss << bytes.size() << " bytes : " << (int)*itr;
		}

		return ss.str();
	}

	int getOpcodeSize() {
		return bytes.size();
	}
};


bool InstructionCompare(Instruction &insa, Instruction &insb)
{
	
	struct emu_cpu_instruction a = insa.getInstruction().cpu;
	struct emu_cpu_instruction b = insb.getInstruction().cpu;

	
	if((a.opc == b.opc) && (a.modrm.mod == b.modrm.mod) && (a.modrm.opc == b.modrm.opc) && (a.modrm.sib.base == b.modrm.sib.base)
			&& (a.modrm.sib.index == b.modrm.sib.index) && (a.modrm.sib.scale == b.modrm.sib.scale) && (a.imm == b.imm) 
			&& (a.imm8 == b.imm8) && (a.imm16 == b.imm16) && (a.disp == b.disp)) {
	
	} else {
		printf("a.opc == b.opc : %d == %d\n", (int)a.opc, (int)b.opc);
;		printf("a.modrm.mod == b.modrm.mod : %d == %d\n", (int)a.modrm.mod, (int)b.modrm.mod);
		printf("a.modrm.opc == b.modrm.opc : %d == %d\n", (int)a.modrm.opc, (int)b.modrm.opc);
		printf("a.modrm.sib.base == b.modrm.sib.base : %d == %d\n", (int)a.modrm.sib.base, (int)b.modrm.sib.base);;
		printf("a.modrm.sib.index == b.modrm.sib.index : %d == %d\n", (int)a.modrm.sib.index,  (int)b.modrm.sib.index);
		printf("a.modrm.sib.scale == a.modrm.sib.scale : %d == %d\n", (int)a.modrm.sib.scale, (int)b.modrm.sib.scale);
		printf("a.imm == b.imm : %d == %d\n", (int)a.imm, (int)b.imm);
		printf("a.imm8 == b.imm8 : %d == %d\n", (int)a.imm8, (int)b.imm8);
		printf("a.imm16 == b.imm16 : %d == %d\n", (int)a.imm16, (int)b.imm16);
		printf("a.disp == b.disp : %d == %d\n", (int)a.disp, (int)b.disp);
	}
	return (a.opc == b.opc) && (a.modrm.mod == b.modrm.mod) && (a.modrm.opc == b.modrm.opc) && (a.modrm.sib.base == b.modrm.sib.base)
			&& (a.modrm.sib.index == b.modrm.sib.index) && (a.modrm.sib.scale == b.modrm.sib.scale)  /*&& (a.imm == b.imm)*/ 
			&& (a.imm8 == b.imm8) && (a.imm16 == b.imm16) && (a.disp == b.disp);  
	

}

void printProgram(vector<BYTE> program) 
{
	for(vector<BYTE>::iterator itr = program.begin(); itr != program.end(); ++itr) {
		cout << hex << setfill('0') << setw(2) << (int)*itr << " ";
	}

	cout << endl;
}

void printInstructions(vector<Instruction> instructions, vector<BYTE> program)
{
	stringstream ss;
	for(vector<Instruction>::iterator itr = instructions.begin(); itr != instructions.end(); ++itr) {
		int startIndex = itr->getStartIndex();
		int endIndex = itr->getEndIndex();
		ss << "[";
		for(int i = startIndex; i <= endIndex; i++) {
			ss << hex << setfill('0') << setw(2) <<  (int)program.at(i) << "";
		}
		ss << "]";
	}

	cout << ss.str() << endl;
}


int main(int argc, char* argv[]) {

	if(argc < 2) {
		cerr << "Error. Usage: program <file path>" << endl;
		return 1;
	}

	
	cout << "Building startingBytes" << endl;
	vector<pair<Opcode, int> > startingBytes;
	/*
	Opcode *a;
	int counter = 0;
	for(int prefix = -1; prefix < 0; prefix++) {

		for(int firstOpc = -1; firstOpc < 256; firstOpc++) {

			for(int secondOpc = 0; secondOpc < 256; secondOpc++) {

				a = new Opcode();
				
				if(firstOpc == -1 && prefix == -1) {
					a->bytes.push_back(secondOpc);
					startingBytes.push_back(make_pair(*a, -1));
					
					delete a;

				} else if(firstOpc != -1 && prefix == -1) {
					a->bytes.push_back(secondOpc);
					a->bytes.push_back(firstOpc);
					startingBytes.push_back(make_pair(*a, -1));
					
					delete a;

				} else if(firstOpc != -1 && prefix != -1) {
					a->bytes.push_back(secondOpc);
					a->bytes.push_back(firstOpc);
					a->bytes.push_back(prefix);
					startingBytes.push_back(make_pair(*a, -1));
		
					delete a;
				
				}
			}
		}
	}

	cout << "Finished building startingBytes" << endl;

	//for(vector<pair<Opcode, int> >::iterator itr = startingBytes.begin(); itr != startingBytes.end(); ++itr) {
	//	cout << itr->first.getOpcodeString() << endl;
	//}


	cout << "startingBytes[65791] = " << startingBytes.at(65791).first.getOpcodeString() << endl;
	cout << "Size of startingBytes is " << startingBytes.size() << endl;
	*/

	Opcode a;
	a.bytes.push_back(05);
	startingBytes.push_back(make_pair(a, -1));
	
	
	Parser p;

	Emulator e;
										
	vector<BYTE> hepProgram = p.parseFile(argv[1]);
	vector<BYTE> mepProgram = hepProgram;
	printProgram(hepProgram);


	e.loadProgramInMemory(hepProgram.data(), hepProgram.size());
	vector<Instruction> hep = e.getInstructionVector();


	//cout << "Got " << hep.size() << " instructions." << endl;

	int validBytesHep = p.parseUntilInvalid(hepProgram.data(), hepProgram.size());

	
	int max = 0;
	
	for(vector<pair<Opcode, int> >::iterator startingBytesIterator = startingBytes.begin(); startingBytesIterator != startingBytes.end(); ++startingBytesIterator) {
		
		int numberOfStartingBytes = startingBytesIterator->first.getOpcodeSize();
		mepProgram.insert(mepProgram.begin(), startingBytesIterator->first.bytes.begin(), startingBytesIterator->first.bytes.end());

		int validBytesMep = p.parseUntilInvalid(mepProgram.data(), mepProgram.size());

		e.loadProgramInMemory(mepProgram.data(), validBytesMep);

		vector<Instruction> mep = e.getInstructionVector();

		if(validBytesMep <= validBytesHep) {
			mepProgram.erase(mepProgram.begin(), mepProgram.begin() + numberOfStartingBytes);
			continue;
		}
		
		/*
		printProgram(mepProgram);

		if(InstructionCompare(hep.back(), mep.back())) {
			cout << "Last is equal" << endl;
		}

		while(!hep.empty()) {


			cout << "HEP: ";
			printInstructions(hep, hepProgram);
			
			cout << "MEP: ";
			printInstructions(mep, mepProgram);

			if(!InstructionCompare(hep.back(), mep.back())) {
				cout << "No synced" << endl;
				break;
			}

			
			hep.pop_back();
			mep.pop_back();
		}

		cout << "Synced after " << dec << hep.size() << " instructions in HEP and " <<  dec << mep.size() << " in MEP" << endl;
		*/

		for(vector<Instruction>::iterator itr = mep.begin(); itr != mep.end(); ++itr) {
			if(!itr->isLegal()) {

				startingBytesIterator->second = distance(mep.begin(), itr);
				
				if(distance(mep.begin(), itr) > max) {
					max = distance(mep.begin(), itr);
				}

				break;
			}

		}	
		
		mepProgram.erase(mepProgram.begin(), mepProgram.begin() + numberOfStartingBytes);

	}



	
	cout << "A maximum of" << dec <<  max << " instructions was found" << endl;

	for(vector<pair<Opcode, int> >::iterator itr = startingBytes.begin(); itr != startingBytes.end(); ++itr) {
		if(itr->second == max) {
			cout << "Opcode " << itr->first.getOpcodeString() << " generated maximum number of valid bytes." << endl;
		}
	}
	

	cout << "Done" << endl;
	
	
	return 0;
	
	
}