#include "Driver.h"
#include "Parser.h"

inline int isBEXTR(char* instruction) {
	UNREFERENCED_PARAMETER(instruction); // <-- LINE TO REMOVE DURING IMPLEMENTATION
	return FALSE;
}

int __stdcall BEXTRInstructionHandler(
	char** instruction,		   // bytes of instruction
	CALLER_CONTEXT* context)   // caller context 
{
	UNREFERENCED_PARAMETER(context); // <-- LINE TO REMOVE DURING IMPLEMENTATION
	if (isBEXTR(*instruction)) {
		// Parse and emulate
		return TRUE;
	}
	else
		return FALSE; // not supported -> not handled
}
