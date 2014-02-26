#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <udis86.h>
#include "instruction.h"

class Parser {
public:
	typedef unsigned char BYTE;
	Parser();
	std::vector<Instruction> parseFile(char fileName[]);
	int parseUntilInvalid(BYTE input[], int len);

private:
	ud_t ud_obj;

};

#endif