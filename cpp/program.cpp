/**
* program.cpp
*/

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


/**
* Opcode is a struct used for saving starting bytes
*/
struct Opcode 
{
	vector<BYTE> bytes;

	/**
	* Returns a string with the starting bytes in hexadecimal form
	*/
	string getOpcodeString() {
		stringstream ss;
		for(vector<BYTE>::iterator itr = bytes.begin(); itr != bytes.end(); ++itr) {
			ss << hex << setfill('0') << setw(2) << (int)*itr;
		}

		return ss.str();
	}

	/**
	* Returns the number of starting bytes in the Opcode struct
	*/
	int getOpcodeSize() {
		return bytes.size();
	}
};

/**
* OpcodeMetaData is a struct for saving various information about an Opcode struct
*/
struct OpcodeMetaData
{

	/**
	* Creates an OpcodeMetaData struct
	*/
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

	/**
	* Calculates the number of registers used for memory access by a MEP created with an Opcode struct
	* @return the number of registers
	*/
	int getNumberOfRecurringRegisters() {
		int min = -1;

		for(int i = 0; i < 8; i++) {
			if(usedRegs[i] > min) {
				min = usedRegs[i];
			}
		}

		return min;
	}

	/**
	* Prints the all purpose registers and how many times they are used for memory access
	*/
	void printRecurringRegisters() {
		for(int i = 0; i < 8; i++) {
			printf("usedRegs[%d] = %d\n", i, usedRegs	[i]);
		}
	}	
};

/**
* Compares two instructions a and b based on their bytes
* @param insa Instruction a
* @param insb Instruction b
* @return true if the instructions are equal, false otherwise
*/
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


/**
* Prints a program in its hexadecimal form
* @param program the program to print
*/
void printProgram(vector<BYTE> program) 
{
	for(vector<BYTE>::iterator itr = program.begin(); itr != program.end(); ++itr) {
		cout << hex << setfill('0') << setw(2) << (int)*itr << " ";
	}

	cout << endl;
}

/**
* Prints a vector in its hexadecimal form with the instructions divided
* @param instructions the instructions to print
*/
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

/**
* Build a vector of pair<Opcode, OpcodeMetaData> objects containing all possible starting bytes between 0x00 and 0xFFFFFF
* @return the vector of Opcode and OpcodeMetaData pairs 
*/
vector<pair<Opcode, OpcodeMetaData> > buildStartingBytesVector()
{
	vector<pair<Opcode, OpcodeMetaData> > startingBytes;
	
	Opcode *a;
	Emulator e;

	int counter = 0;
	for(int prefix = -1; prefix < 256; prefix++) {

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

/**
* Takes a vector of Instruction objects and returns is as a vector of bytes
* @param instructions the vector of Instruction objects
* @return a vector of bytes
*/
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

// int checkSyncConditionsAndReturnMax(Parser &p, Emulator &e, vector<Instruction> &hep, vector<BYTE> &mepProgram, vector<pair<Opcode, OpcodeMetaData> > &startingBytes)
// {

// 	int validBytesHep = p.parseUntilInvalid(getBytesFromInstructions(hep));
// 	int maxSyncedAfter = 0;
// 	for(vector<pair<Opcode, OpcodeMetaData> >::iterator startingBytesIterator = startingBytes.begin(); startingBytesIterator != startingBytes.end(); ++startingBytesIterator) {
		
// 		vector<Instruction> tempHep = hep;
// 		int numberOfStartingBytes = startingBytesIterator->first.getOpcodeSize();
// 		mepProgram.insert(mepProgram.begin(), startingBytesIterator->first.bytes.begin(), startingBytesIterator->first.bytes.end());
// 		//printProgram(mepProgram);
// 		int validBytesMep = p.parseUntilInvalid(mepProgram);

// 		e.loadProgramInMemory(mepProgram);

// 		vector<Instruction> mep = e.getInstructionVector();
// 		vector<Instruction> tempMep = mep;

// 		if(validBytesMep <= validBytesHep) {
// 			mepProgram.erase(mepProgram.begin(), mepProgram.begin() + numberOfStartingBytes);
// 			continue;
// 		}
		
// 		int hepNumberOfInstructions = hep.size();
// 		int poppedInstructionCounter = 0;
		
// 		//printProgram(mepProgram);
// 		// Check for sync, number of non-synced instructions will be in mep.size(). 
// 		while(!tempHep.empty() && !tempMep.empty()) {

// 			// Print instructions
// 			// cout << "HEP: ";
// 			// printInstructions(tempHep);
			
// 			// cout << "MEP: ";
// 			// printInstructions(tempMep);
			
// 			if(!InstructionCompare(tempHep.back(), tempMep.back())) {
// 				break;
// 			}
// 			tempHep.pop_back();
// 			tempMep.pop_back();
// 			poppedInstructionCounter++;
// 		}

// 		int numberOfHiddenInstructions = hepNumberOfInstructions - poppedInstructionCounter;

// 		startingBytesIterator->second.syncNumber = numberOfHiddenInstructions;

// 		if(numberOfHiddenInstructions > maxSyncedAfter && numberOfHiddenInstructions < hepNumberOfInstructions) {
// 			maxSyncedAfter = numberOfHiddenInstructions;
// 		}
	
		
// 		mepProgram.erase(mepProgram.begin(), mepProgram.begin() + numberOfStartingBytes);
// 	}

// 	return maxSyncedAfter;
// }


/**
* Inserts the starting bytes in an Opcode object before the HEP, checks when the MEP synchronizes with the HEP 
* and returns the number of hidden instructions
* @param p the Parser object used to check if the new MEP contains invalid instructions
* @param e the Emulator object used to build the new MEP
* @param hep the HEP in the form of Instruction objects
* @param mepProgram the MEP in the form of bytes
* @param opc the Opcode object containing the starting bytes
* @param opc_meta the OpcodeMetaData object containing information about the Opcode object
* @return the number of hidden instructions
*/
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

// int calculateRecurringRegistersAndReturnMin(Emulator &e, vector<Instruction> &hep, vector<BYTE> &mepProgram, vector<pair<Opcode, OpcodeMetaData> > &startingBytes) 
// {
// 	int minNumberOfRecurringRegisters = 1024;

// 	for(vector<pair<Opcode, OpcodeMetaData> >::iterator startingBytesIterator = startingBytes.begin(); startingBytesIterator != startingBytes.end(); ++startingBytesIterator) {
// 		Opcode oc = startingBytesIterator->first;
// 		int opcodeSize = oc.getOpcodeSize();
// 		mepProgram.insert(mepProgram.begin(), oc.bytes.begin(), oc.bytes.end());
// 		e.loadProgramInMemory(mepProgram);
// 		vector<Instruction> mep = e.getInstructionVector();


// 		for(vector<Instruction>::iterator itr = mep.begin(); itr != mep.end(); ++itr) {
// 			struct emu_instruction ins = itr->getInstruction();
// 			struct emu_cpu_instruction_info info = itr->getInstructionInfo();

// 			if(ins.cpu.modrm.mod != 0xc0 && info.format.modrm_byte != 0) {
// 				startingBytesIterator->second.usedRegs[ins.cpu.modrm.reg]++;
// 				if(ins.cpu.modrm.rm == 0x4) {
// 					startingBytesIterator->second.usesSib = true;
// 				}
// 			}
// 		}

// 		startingBytesIterator->second.numberOfRecurringRegisters = startingBytesIterator->second.getNumberOfRecurringRegisters();

// 		if(startingBytesIterator->second.numberOfRecurringRegisters < minNumberOfRecurringRegisters) {
// 			minNumberOfRecurringRegisters = startingBytesIterator->second.numberOfRecurringRegisters;
// 		}

// 		mepProgram.erase(mepProgram.begin(), mepProgram.begin() + opcodeSize);
// 	}


// 	return minNumberOfRecurringRegisters;
// }

/**
* Inserts the starting bytes in an Opcode object and calculates how many registers that are used for memory access
* @param e the Emulator object used to build the new MEP
* @param hep the HEP in the form of Instruction objects
* @param mepProgram the MEP in the form of bytes
* @param opc the Opcode object containing the starting bytes
* @param opc_meta the OpcodeMetaData object containing information about the Opcode object
* @return the number of registers used to access memory
*/
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


/**
* The main method responsible for obfuscating a program. Takes an input file, obfuscates it an outputs a template
* for the obfuscated program. The arguments are made up of the path to the input file, the number of instructions
* in the input file and if 32 bit mov substitution should be used.
*/
int main(int argc, char* argv[]) {

	if(argc < 4) {
		cerr << "Error. Usage: program <file path> <number of instructions> <Replace 32 bit mov or not>" << endl;
		return 1;
	}

	
	//cout << "Building startingBytes" << endl;
	vector<pair<Opcode, OpcodeMetaData> > startingBytes = buildStartingBytesVector();

	//cout << "Finished building startingBytes" << endl;
	
	Parser p;

	Emulator e;

	int numberOfInstructions = atoi(argv[2]);
										
	vector<BYTE> program = p.parseFile(argv[1], numberOfInstructions);

	vector<Instruction> preface;
	vector<Instruction> hep;

	e.doPreface(program, preface, hep);

	// Replace 32 bit mov or not
	if(atoi(argv[3]) == 1) {
		hep = e.optimizeHep(hep);
	}


	vector<pair<Opcode, OpcodeMetaData> > oldStartingBytes = startingBytes;

	
	vector<BYTE> mepProgram = getBytesFromInstructions(hep);
	//printProgram(hepProgram);


	//printInstructions(hep);

	
	//cout << "Got " << hep.size() << " instructions." << endl;
	bool foundOkayStuff = false;
	vector<pair<Opcode, OpcodeMetaData> > goodStartingBytes;
	
	do {
		cout << "Checking for best sync conditions" << endl;

		goodStartingBytes.clear();
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

		if(goodStartingBytes.size() == 0) {
			startingBytes = oldStartingBytes;
			mepProgram.insert(mepProgram.begin(), 0x90);
			foundOkayStuff = false;
		} else {
			foundOkayStuff = true;
		}

	} while(!foundOkayStuff);

	startingBytes = goodStartingBytes;

	printf("startingBytes.size() = %d\n", startingBytes.size());
	
	Opcode bestStartingOpcode = startingBytes.back().first;
	//startingBytes.back().second.printRecurringRegisters();
	mepProgram.insert(mepProgram.begin(), bestStartingOpcode.bytes.begin(), bestStartingOpcode.bytes.end());
	e.loadProgramInMemory(mepProgram);
	vector<Instruction> mep = e.getInstructionVector();

	printInstructions(mep);

	cout << bestStartingOpcode.getOpcodeString();
	printf("Which has %d bytes\n", bestStartingOpcode.getOpcodeSize());

	vector<Instruction> memoryAdjustment;
	e.adjustForMemoryAccess(memoryAdjustment, mep);

	p.parseAndPrintPreface(getBytesFromInstructions(preface)); 
	p.parseAndPrintMemoryAdjustment(getBytesFromInstructions(memoryAdjustment)); 
	p.parseAndPrintProgram(getBytesFromInstructions(mep), bestStartingOpcode.getOpcodeSize(), 0);
	p.printProgramReturn();
	
	
	return 0;
	
	
}