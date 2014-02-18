#include "cpu.h"

Cpu::Cpu() 
{
	//Stuff happens and stuff
}

bool Cpu::storeByteInRegistry(int8_t value, int registry) 
{
	switch(registry) {
		case EAX:
			eax = value;
			break;
		case ECX:
			ecx = value;
			break;
		case EDX:
			edx = value;
			break;
		case EBX:
			ebx = value;
			break;
		case ESI:
			esi = value;
			break;
		case EDI:
			edi = value;
			break;
		case ESP:
			return false; // you shall not store stuff in ESP!
		case EBP:
			return false; // you shall not store stuff in EBP!
	}

	return true;
}

bool Cpu::storeDoubleWordInRegistry(int32_t value, int registry) 
{
	switch(registry) {
		case EAX:
			eax = value;
			break;
		case ECX:
			ecx = value;
			break;
		case EDX:
			edx = value;
			break;
		case EBX:
			ebx = value;
			break;
		case ESI:
			esi = value;
			break;
		case EDI:
			edi = value;
			break;
		case ESP:
			return false; // you shall not store stuff in ESP!
		case EBP:
			return false; // you shall not store stuff in EBP!
	}

	return true;
}