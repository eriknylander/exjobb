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

struct OpcodeMetaData
{

	OpcodeMetaData() {
		for (int i = 0; i < 8; i++) {
			usedRegs[i] = 0;
		}

		usesSib = false;
	}

	char usedRegs[8];
	int syncNumber;
	int numberOfRecurringRegisters;
	bool usesSib;
	int getNumberOfRecurringRegisters() {
		int min = -1;

		for(int i = 0; i < 8; i++) {
			if(usedRegs[i] > min) {
				min = usedRegs[i];
			}
		}

		return min;
	}

	void printRecurringRegisters() {
		for(int i = 0; i < 8; i++) {
			printf("usedRegs[%d] = %d\n", i, usedRegs	[i]);
		}
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

vector<pair<Opcode, OpcodeMetaData> > buildStartingBytesVector()
{
	vector<pair<Opcode, OpcodeMetaData> > startingBytes;
	
	Opcode *a;
	Emulator e;

	int counter = 0;
	for(int prefix = -1; prefix < 0; prefix++) {

		for(int firstOpc = -1; firstOpc < 256; firstOpc++) {

			for(int secondOpc = 0; secondOpc < 256; secondOpc++) {

				a = new Opcode();
				OpcodeMetaData omd;
				omd.syncNumber = -1;
				Instruction ins;
			
				
				if(firstOpc == -1 && prefix == -1) {
					a->bytes.push_back(secondOpc);
					
					if(e.getInstruction(a->bytes, ins) && ins.getBytes().size() > a->getOpcodeSize() && secondOpc != 0xf6) {
						startingBytes.push_back(make_pair(*a, omd));
					}
					
					delete a;


				} else if(firstOpc != -1 && prefix == -1) {
					a->bytes.push_back(firstOpc);
					a->bytes.push_back(secondOpc);

					bool gotInstruction = e.getInstruction(a->bytes, ins);
					

					
					if(gotInstruction && ins.getBytes().size() > a->getOpcodeSize() && firstOpc != 0xf6) {
						startingBytes.push_back(make_pair(*a, omd));
					} 

					delete a;

				} else if(firstOpc != -1 && prefix != -1) {
					a->bytes.push_back(prefix);
					a->bytes.push_back(firstOpc);
					a->bytes.push_back(secondOpc);
					bool gotInstruction = e.getInstruction(a->bytes, ins);
					if(gotInstruction&& ins.getBytes().size() > a->getOpcodeSize() && prefix != 0xf6) {
						startingBytes.push_back(make_pair(*a, omd));
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

int checkSyncConditionsAndReturnMax(Parser &p, Emulator &e, vector<Instruction> &hep, vector<BYTE> &mepProgram, vector<pair<Opcode, OpcodeMetaData> > &startingBytes)
{

	int validBytesHep = p.parseUntilInvalid(getBytesFromInstructions(hep));
	int maxSyncedAfter = 0;
	for(vector<pair<Opcode, OpcodeMetaData> >::iterator startingBytesIterator = startingBytes.begin(); startingBytesIterator != startingBytes.end(); ++startingBytesIterator) {
		
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

		startingBytesIterator->second.syncNumber = numberOfHiddenInstructions;

		if(numberOfHiddenInstructions > maxSyncedAfter && numberOfHiddenInstructions < hepNumberOfInstructions) {
			maxSyncedAfter = numberOfHiddenInstructions;
		}
	
		
		mepProgram.erase(mepProgram.begin(), mepProgram.begin() + numberOfStartingBytes);
	}

	return maxSyncedAfter;
}

int checkSyncConditionsAndReturnSyncNumber(Parser &p, Emulator &e, vector<Instruction> &hep, vector<BYTE> &mepProgram, Opcode &opc, OpcodeMetaData &opc_meta)
{
	int validBytesHep = p.parseUntilInvalid(getBytesFromInstructions(hep));
	vector<Instruction> tempHep = hep;
	int numberOfStartingBytes = opc.getOpcodeSize();
	mepProgram.insert(mepProgram.begin(), opc.bytes.begin(), opc.bytes.end());
	//printProgram(mepProgram);
	int validBytesMep = p.parseUntilInvalid(mepProgram);

	e.loadProgramInMemory(mepProgram);

	vector<Instruction> mep = e.getInstructionVector();
	vector<Instruction> tempMep = mep;

	if(validBytesMep <= validBytesHep) {
		mepProgram.erase(mepProgram.begin(), mepProgram.begin() + numberOfStartingBytes);
		return -1;
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

	opc_meta.syncNumber = numberOfHiddenInstructions;

	mepProgram.erase(mepProgram.begin(), mepProgram.begin() + numberOfStartingBytes);

	return numberOfHiddenInstructions;
}

int calculateRecurringRegistersAndReturnMin(Emulator &e, vector<Instruction> &hep, vector<BYTE> &mepProgram, vector<pair<Opcode, OpcodeMetaData> > &startingBytes) 
{
	int minNumberOfRecurringRegisters = 1024;

	for(vector<pair<Opcode, OpcodeMetaData> >::iterator startingBytesIterator = startingBytes.begin(); startingBytesIterator != startingBytes.end(); ++startingBytesIterator) {
		Opcode oc = startingBytesIterator->first;
		int opcodeSize = oc.getOpcodeSize();
		mepProgram.insert(mepProgram.begin(), oc.bytes.begin(), oc.bytes.end());
		e.loadProgramInMemory(mepProgram);
		vector<Instruction> mep = e.getInstructionVector();


		for(vector<Instruction>::iterator itr = mep.begin(); itr != mep.end(); ++itr) {
			struct emu_instruction ins = itr->getInstruction();
			struct emu_cpu_instruction_info info = itr->getInstructionInfo();

			if(ins.cpu.modrm.mod != 0xc0 && info.format.modrm_byte != 0) {
				startingBytesIterator->second.usedRegs[ins.cpu.modrm.reg]++;
				if(ins.cpu.modrm.rm == 0x4) {
					startingBytesIterator->second.usesSib = true;
				}
			}
		}

		startingBytesIterator->second.numberOfRecurringRegisters = startingBytesIterator->second.getNumberOfRecurringRegisters();

		if(startingBytesIterator->second.numberOfRecurringRegisters < minNumberOfRecurringRegisters) {
			minNumberOfRecurringRegisters = startingBytesIterator->second.numberOfRecurringRegisters;
		}

		mepProgram.erase(mepProgram.begin(), mepProgram.begin() + opcodeSize);
	}


	return minNumberOfRecurringRegisters;
}

int calculateRecurringRegisters(Emulator &e, vector<Instruction> &hep, vector<BYTE> &mepProgram, Opcode &opc, OpcodeMetaData &opc_meta)
{
	
	int opcodeSize = opc.getOpcodeSize();
	mepProgram.insert(mepProgram.begin(), opc.bytes.begin(), opc.bytes.end());
	e.loadProgramInMemory(mepProgram);
	vector<Instruction> mep = e.getInstructionVector();


	for(vector<Instruction>::iterator itr = mep.begin(); itr != mep.end(); ++itr) {
		struct emu_instruction ins = itr->getInstruction();
		struct emu_cpu_instruction_info info = itr->getInstructionInfo();

		if(ins.cpu.modrm.mod != 0xc0 && info.format.modrm_byte != 0) {
			opc_meta.usedRegs[ins.cpu.modrm.reg]++;
			if(ins.cpu.modrm.rm == 0x4) {
				opc_meta.usesSib = true;
			}
		}
	}

	opc_meta.numberOfRecurringRegisters = opc_meta.getNumberOfRecurringRegisters();


	mepProgram.erase(mepProgram.begin(), mepProgram.begin() + opcodeSize);

	return opc_meta.numberOfRecurringRegisters;
}




int main(int argc, char* argv[]) {

	if(argc < 2) {
		cerr << "Error. Usage: program <file path>" << endl;
		return 1;
	}

	
	//cout << "Building startingBytes" << endl;
	vector<pair<Opcode, OpcodeMetaData> > startingBytes = buildStartingBytesVector();
	//cout << "Finished building startingBytes" << endl;
	
	Parser p;

	Emulator e;
										
	vector<BYTE> program = p.parseFile(argv[1]);

	vector<Instruction> preface;
	vector<Instruction> hep;

	e.doPreface(program, preface, hep);

	// cout << "Preface" << endl;
	// printInstructions(preface);
	// cout << "Hep" << endl;
	// printInstructions(hep);


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
	//int maxSyncedAfter = checkSyncConditionsAndReturnMax(p, e, hep, mepProgram, startingBytes);
	int maxSyncedAfter = 0;
	for(vector<pair<Opcode, OpcodeMetaData> >::iterator startingBytesIterator = startingBytes.begin(); startingBytesIterator != startingBytes.end(); ++startingBytesIterator) {
		int syncNumber = checkSyncConditionsAndReturnSyncNumber(p, e, hep, mepProgram, startingBytesIterator->first, startingBytesIterator->second);

		if(syncNumber > maxSyncedAfter) {
			maxSyncedAfter = syncNumber;
		}
		
	}

 	cout << "Done checking sync conditions" << endl;


	cout << "Removing startingBytes with bad sync number" << endl;
	//printf("A maximum of %d instructions were found.\n", maxSyncedAfter);
	vector<pair<Opcode, OpcodeMetaData> > goodStartingBytes;
	
	for(vector<pair<Opcode, OpcodeMetaData> >::iterator itr = startingBytes.begin(); itr != startingBytes.end(); ++itr) {
		if(itr->second.syncNumber == maxSyncedAfter) {
			goodStartingBytes.push_back(*itr);
			//cout << "Opcode " << itr->first.getOpcodeString() << " generated maximum number of valid bytes." << endl;
		}
	}

	printf("Size of goodStartingBytes = %d\n", goodStartingBytes.size());

	cout << "Done removing startingBytes" << endl;

	startingBytes = goodStartingBytes;
	goodStartingBytes.clear();

	printf("maxSyncedAfter = %d\n", maxSyncedAfter);
	printProgram(getBytesFromInstructions(hep));

	// TODO: Make progam choose starting bytes in some clever way.

	printf("Calculating recurring registers and usage of sib bytes\n");

	//int minNumberOfRecurringRegisters = calculateRecurringRegistersAndReturnMin(e, hep, mepProgram, startingBytes);

	int minNumberOfRecurringRegisters = 1024;
	for(vector<pair<Opcode, OpcodeMetaData> >::iterator startingBytesIterator = startingBytes.begin(); startingBytesIterator != startingBytes.end(); ++startingBytesIterator) {
		int numberOfRecurringRegisters = calculateRecurringRegisters(e, hep, mepProgram, startingBytesIterator->first, startingBytesIterator->second);

		if(numberOfRecurringRegisters < minNumberOfRecurringRegisters) {
			minNumberOfRecurringRegisters = numberOfRecurringRegisters;
		}
	}


	for(vector<pair<Opcode, OpcodeMetaData> >::iterator startingBytesIterator = startingBytes.begin(); startingBytesIterator != startingBytes.end(); ++startingBytesIterator) {
		if(startingBytesIterator->second.numberOfRecurringRegisters == minNumberOfRecurringRegisters && !startingBytesIterator->second.usesSib) {
			goodStartingBytes.push_back(make_pair(startingBytesIterator->first, startingBytesIterator->second));
		}
	}

	startingBytes = goodStartingBytes;

	printf("startingBytes.size() = %d\n", startingBytes.size());
	
	Opcode bestStartingOpcode = startingBytes.back().first;
	//startingBytes.back().second.printRecurringRegisters();
	mepProgram.insert(mepProgram.begin(), bestStartingOpcode.bytes.begin(), bestStartingOpcode.bytes.end());
	e.loadProgramInMemory(mepProgram);
	vector<Instruction> mep = e.getInstructionVector();

	

	vector<Instruction> memoryAdjustment;
	e.adjustForMemoryAccess(memoryAdjustment, mep);

	printProgram(getBytesFromInstructions(preface));
	p.parseAndPrintProgram(getBytesFromInstructions(preface), getBytesFromInstructions(memoryAdjustment), mepProgram, bestStartingOpcode.bytes.size());
	
	
	return 0;
	
	
}