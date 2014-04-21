#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <udis86.h>
#include "instruction.h"

class Parser {
public:
	typedef unsigned char BYTE;
	Parser();
	std::vector<BYTE> parseFile(char fileName[], int numberOfInstructions);
	int parseUntilInvalid(std::vector<BYTE> buffer);
	void parseAndPrintPreface(std::vector<BYTE> preface);
	void parseAndPrintMemoryAdjustment(std::vector<BYTE> memoryAdjustment);
	void parseAndPrintProgram(std::vector<BYTE> mep, int startingOpcodeSize, int index);
	void printProgramReturn();

private:
	ud_t ud_obj;

};

#endif