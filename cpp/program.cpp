#include <iostream>
#include <iomanip>
#include <utility>
#include <fstream>
#include <vector>
#include <iterator>
#include <sstream>
#include <algorithm>
#include "instruction.h"
#include "parser.h"
#include "emulator.h"


typedef unsigned char BYTE;

using namespace std;

struct Opcode 
{
	vector<BYTE> bytes;
	string getOpcodeString() {
		stringstream ss;
		for(vector<BYTE>::iterator itr = bytes.begin(); itr != bytes.end(); ++itr) {
			ss << hex << setfill('0') << setw(2) << (int)*itr;
		}

		return ss.str();
	}

	int getOpcodeSize() {
		return bytes.size();
	}
};



bool InstructionCompare(Instruction &insa, Instruction &insb) {
	
	vector<BYTE> aBytes = insa.getBytes();
	vector<BYTE> bBytes = insb.getBytes();

	if(aBytes.size()!= bBytes.size()) {
		return false;
	}

	vector<BYTE>::iterator aItr = aBytes.begin();
	vector<BYTE>::iterator bItr = bBytes.begin();

	while(aItr != aBytes.end() && bItr != bBytes.end()) {
		if(*aItr != *bItr) {
			//printf("%02x != %02x returning false", *aItr, *bItr);
			return false;
		}

		++aItr;
		++bItr;
	}

	// cout << "Returning true" << endl;
	return true;
	
}

void printProgram(vector<BYTE> program) 
{
	for(vector<BYTE>::iterator itr = program.begin(); itr != program.end(); ++itr) {
		cout << hex << setfill('0') << setw(2) << (int)*itr << " ";
	}

	cout << endl;
}

void printInstructions(vector<Instruction> instructions)
{
	stringstream ss;
	for(vector<Instruction>::iterator itr = instructions.begin(); itr != instructions.end(); ++itr) {
		ss << "[";
		ss << itr->getByteString();
		ss << "]";
	}

	cout << ss.str() << endl;
}

vector<pair<Opcode, int> > buildStartingBytesVector()
{
	vector<pair<Opcode, int> > startingBytes;
	
	Opcode *a;
	Emulator e;

	int counter = 0;
	for(int prefix = -1; prefix < 0; prefix++) {

		for(int firstOpc = -1; firstOpc < 256; firstOpc++) {

			for(int secondOpc = 0; secondOpc < 256; secondOpc++) {

				a = new Opcode();
				Instruction ins;
			
				
				if(firstOpc == -1 && prefix == -1) {
					a->bytes.push_back(secondOpc);
					
					if(e.getInstruction(a->bytes, ins) && ins.getBytes().size() > a->getOpcodeSize() && secondOpc != 0xf6) {
						startingBytes.push_back(make_pair(*a, -1));
					}
					
					delete a;


				} else if(firstOpc != -1 && prefix == -1) {
					a->bytes.push_back(firstOpc);
					a->bytes.push_back(secondOpc);

					bool gotInstruction = e.getInstruction(a->bytes, ins);
					

					
					if(gotInstruction && ins.getBytes().size() > a->getOpcodeSize() && firstOpc != 0xf6) {
						startingBytes.push_back(make_pair(*a, -1));
					} 

					delete a;

				} else if(firstOpc != -1 && prefix != -1) {
					a->bytes.push_back(prefix);
					a->bytes.push_back(firstOpc);
					a->bytes.push_back(secondOpc);
					bool gotInstruction = e.getInstruction(a->bytes, ins);
					if(gotInstruction&& ins.getBytes().size() > a->getOpcodeSize() && prefix != 0xf6) {
						startingBytes.push_back(make_pair(*a, -1));
					} 

					
		
					delete a;
				}
			}
		}
	}

	return startingBytes;	
}

vector<BYTE> getBytesFromInstructions(vector<Instruction> instructions)
{
	vector<BYTE> byteVector;
	for(vector<Instruction>::reverse_iterator itr = instructions.rbegin(); itr != instructions.rend(); ++itr) {
		vector<BYTE> bytes = itr->getBytes();
		byteVector.insert(byteVector.begin(), bytes.begin(), bytes.end());
		//cout << "Inserting " << itr->getByteString() << endl;
	}

	return byteVector;
}





int main(int argc, char* argv[]) {

	if(argc < 2) {
		cerr << "Error. Usage: program <file path>" << endl;
		return 1;
	}

	
	//cout << "Building startingBytes" << endl;
	vector<pair<Opcode, int> > startingBytes = buildStartingBytesVector();
	//cout << "Finished building startingBytes" << endl;
	
	Parser p;

	Emulator e;
										
	vector<BYTE> program = p.parseFile(argv[1]);

	vector<Instruction> preface;
	vector<Instruction> hep;

	e.doPreface(program, preface, hep);

	cout << "Preface" << endl;
	printInstructions(preface);
	cout << "Hep" << endl;
	printInstructions(hep);


	//Opcode a;
	//a.bytes.push_back(0xf6); 
	// a.bytes.push_back(0xf3); 
	// a.bytes.push_back(0xf7);
	// // a.bytes.push_back(0x83); 
	// // a.bytes.push_back(0xc6); 
	// // a.bytes.push_back(0x01); 
	// // a.bytes.push_back(0x31); 
	// // a.bytes.push_back(0xc0); 
	// // a.bytes.push_back(0x89); 
	// // a.bytes.push_back(0x76); 
	// // a.bytes.push_back(0x08); 
	// // a.bytes.push_back(0x89);

	// Instruction pre;

	// e.getInstruction(a.bytes, pre);

	//startingBytes.push_back(make_pair(a, -1));


	vector<BYTE> mepProgram = getBytesFromInstructions(hep);
	//printProgram(hepProgram);


	printInstructions(hep);


	//cout << "Got " << hep.size() << " instructions." << endl;


	cout << "Checking for best sync conditions" << endl;
	int validBytesHep = p.parseUntilInvalid(getBytesFromInstructions(hep));
	int maxSyncedAfter = 0;
	
	for(vector<pair<Opcode, int> >::iterator startingBytesIterator = startingBytes.begin(); startingBytesIterator != startingBytes.end(); ++startingBytesIterator) {
		
		vector<Instruction> tempHep = hep;
		int numberOfStartingBytes = startingBytesIterator->first.getOpcodeSize();
		mepProgram.insert(mepProgram.begin(), startingBytesIterator->first.bytes.begin(), startingBytesIterator->first.bytes.end());
		//printProgram(mepProgram);
		int validBytesMep = p.parseUntilInvalid(mepProgram);

		e.loadProgramInMemory(mepProgram);

		vector<Instruction> mep = e.getInstructionVector();
		vector<Instruction> tempMep = mep;

		if(validBytesMep <= validBytesHep) {
			mepProgram.erase(mepProgram.begin(), mepProgram.begin() + numberOfStartingBytes);
			continue;
		}
		
		int hepNumberOfInstructions = hep.size();
		int poppedInstructionCounter = 0;
		
		//printProgram(mepProgram);
		// Check for sync, number of non-synced instructions will be in mep.size(). 
		while(!tempHep.empty() && !tempMep.empty()) {

			// Print instructions
			// cout << "HEP: ";
			// printInstructions(tempHep);
			
			// cout << "MEP: ";
			// printInstructions(tempMep);
			
			if(!InstructionCompare(tempHep.back(), tempMep.back())) {
				break;
			}
			tempHep.pop_back();
			tempMep.pop_back();
			poppedInstructionCounter++;
		}

		int numberOfHiddenInstructions = hepNumberOfInstructions - poppedInstructionCounter;

		startingBytesIterator->second = numberOfHiddenInstructions;

		if(numberOfHiddenInstructions > maxSyncedAfter && numberOfHiddenInstructions < hepNumberOfInstructions) {
			maxSyncedAfter = numberOfHiddenInstructions;
		}

		

		//printf("maxSyncedAfter = %d with opcode ", maxSyncedAfter);
		//cout << startingBytesIterator->first.getOpcodeString() << endl;


		// for(vector<Instruction>::iterator itr = mep.begin(); itr != mep.end(); ++itr) {
		// 	if(!itr->isLegal()) {

		// 		int nbrLegalIns = distance(mep.begin(), itr);
		// 		startingBytesIterator->second = max(nbrLegalIns, (int)tempMep.size());

				
		// 		if(nbrLegalIns > maxBeforeFail) {
		// 			maxBeforeFail = nbrLegalIns;
		// 		}

		// 		break;
		// 	}

		// }	
		
		mepProgram.erase(mepProgram.begin(), mepProgram.begin() + numberOfStartingBytes);
	}
 

 	cout << "Done checking sync conditions" << endl;


	cout << "Removing startingBytes with bad sync number" << endl;
	//printf("A maximum of %d instructions were found.\n", maxSyncedAfter);
	vector<pair<Opcode, int> > goodStartingBytes;
	for(vector<pair<Opcode, int> >::iterator itr = startingBytes.begin(); itr != startingBytes.end(); ++itr) {
		if(itr->second == maxSyncedAfter) {
			// Instruction ins;
			// e.getInstruction(vector<BYTE>(itr->first.bytes.begin(), itr->first.bytes.end()), ins);
			// printf("Prefix: %0d\nOpcode: %02x\n2ndOpcode: %02x\n", ins.getInstruction().cpu.prefixes, ins.getInstruction().cpu.opc, ins.getInstruction().cpu.opc_2nd); 
			// printf("mod = %d\n", ins.getInstruction().cpu.modrm.mod);
			// if(ins.isLegal()) {
			// 	cout << "Opcode " << itr->first.getOpcodeString() << " generated maximum number of valid bytes." << endl;
			// }
			goodStartingBytes.push_back(*itr);
			cout << "Opcode " << itr->first.getOpcodeString() << " generated maximum number of valid bytes." << endl;
		}
	}

	cout << "Size of goodStartingBytes = " << goodStartingBytes.size() << endl;

	cout << "Done removing startingBytes" << endl;

	startingBytes = goodStartingBytes;
		

	printf("maxSyncedAfter = %d\n", maxSyncedAfter);
	printProgram(getBytesFromInstructions(hep));

	// TODO: Make progam choose starting bytes in some clever way.

	Opcode bestStartingOpcode = startingBytes.back().first;
	mepProgram.insert(mepProgram.begin(), bestStartingOpcode.bytes.begin(), bestStartingOpcode.bytes.end());
	e.loadProgramInMemory(mepProgram);
	vector<Instruction> mep = e.getInstructionVector();

	vector<Instruction> memoryAdjustment;
	e.adjustForMemoryAccess(memoryAdjustment, mep);

	printProgram(getBytesFromInstructions(preface));
	p.parseAndPrintProgram(getBytesFromInstructions(preface), getBytesFromInstructions(memoryAdjustment), mepProgram, bestStartingOpcode.bytes.size());
	
	
	return 0;
	
	
}