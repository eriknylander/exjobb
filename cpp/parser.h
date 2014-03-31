#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <udis86.h>
#include "instruction.h"

class Parser {
public:
	typedef unsigned char BYTE;
	Parser();
	std::vector<BYTE> parseFile(char fileName[]);
	int parseUntilInvalid(std::vector<BYTE> buffer);

private:
	ud_t ud_obj;

};

#endif