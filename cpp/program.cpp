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
		for(vector<BYTE>::reverse_iterator itr = bytes.rbegin(); itr != bytes.rend(); ++itr) {
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

vector<pair<Opcode, int> > buildStartingBytesVector()
{
	vector<pair<Opcode, int> > startingBytes;
	
	Opcode *a;
	int counter = 0;
	for(int prefix = -1; prefix < 0; prefix++) {

		for(int firstOpc = -1; firstOpc < 0; firstOpc++) {

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

	return startingBytes;	
}


int main(int argc, char* argv[]) {

	if(argc < 2) {
		cerr << "Error. Usage: program <file path>" << endl;
		return 1;
	}

	
	// cout << "Building startingBytes" << endl;
	vector<pair<Opcode, int> > startingBytes = buildStartingBytesVector();
	// cout << "Finished building startingBytes" << endl;

	
	Parser p;

	Emulator e;
										
	vector<BYTE> hepProgram = p.parseFile(argv[1]);
	vector<BYTE> mepProgram = hepProgram;
	//printProgram(hepProgram);


	e.loadProgramInMemory(hepProgram);
	vector<Instruction> hep = e.getInstructionVector();


	//cout << "Got " << hep.size() << " instructions." << endl;

	int validBytesHep = p.parseUntilInvalid(hepProgram.data(), hepProgram.size());
	int maxSyncedAfter = 0;
	int maxBeforeFail = 0;
	
	for(vector<pair<Opcode, int> >::iterator startingBytesIterator = startingBytes.begin(); startingBytesIterator != startingBytes.end(); ++startingBytesIterator) {
		
		vector<Instruction> tempHep = hep;
		int numberOfStartingBytes = startingBytesIterator->first.getOpcodeSize();
		mepProgram.insert(mepProgram.begin(), startingBytesIterator->first.bytes.rbegin(), startingBytesIterator->first.bytes.rend());

		int validBytesMep = p.parseUntilInvalid(mepProgram.data(), mepProgram.size());

		e.loadProgramInMemory(mepProgram);

		vector<Instruction> mep = e.getInstructionVector();
		vector<Instruction> tempMep = mep;

		if(validBytesMep <= validBytesHep) {
			mepProgram.erase(mepProgram.begin(), mepProgram.begin() + numberOfStartingBytes);
			continue;
		}
		
		int mepNumberOfInstructions = mep.size();
		
		
		//printProgram(mepProgram);
		// Check for sync, number of non-synced instructions will be in mep.size(). 
		while(!tempHep.empty() && !tempMep.empty()) {

			// Print instructions
			 //cout << "HEP: ";
			 //printInstructions(tempHep, hepProgram);
			
			 //cout << "MEP: ";
			 //printInstructions(mep, mepProgram);
			
			if(!InstructionCompare(tempHep.back(), tempMep.back())) {
				break;
			}
			tempHep.pop_back();
			tempMep.pop_back();
		}
		startingBytesIterator->second = tempMep.size();

		if(tempMep.size() > maxSyncedAfter) {
			maxSyncedAfter = tempMep.size();
		}

		//printf("maxSyncedAfter = %d with opcode ", maxSyncedAfter);
		//cout << startingBytesIterator->first.getOpcodeString() << endl;


		for(vector<Instruction>::iterator itr = mep.begin(); itr != mep.end(); ++itr) {
			if(!itr->isLegal()) {

				int nbrLegalIns = distance(mep.begin(), itr);
				startingBytesIterator->second = max(nbrLegalIns, (int)tempMep.size());

				
				if(nbrLegalIns > maxBeforeFail) {
					maxBeforeFail = nbrLegalIns;
				}

				break;
			}

		}	
		
		mepProgram.erase(mepProgram.begin(), mepProgram.begin() + numberOfStartingBytes);
	}
 


	
	//printf("A maximum of %d instructions were found.\n", maxSyncedAfter);

	for(vector<pair<Opcode, int> >::iterator itr = startingBytes.begin(); itr != startingBytes.end(); ++itr) {
		if(itr->second == maxSyncedAfter) {
			cout << "Opcode " << itr->first.getOpcodeString() << " generated maximum number of valid bytes." << endl;
		}
	}
	

	printf("maxSyncedAfter = %d\n", maxSyncedAfter);
	printProgram(hepProgram);
	cout << "Done" << endl;
	
	
	return 0;
	
	
}