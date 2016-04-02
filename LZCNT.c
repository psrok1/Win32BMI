#include "Driver.h"
#include "Parser.h"

inline int isLZCNT(char* instruction) {
	UNREFERENCED_PARAMETER(instruction); // <-- LINE TO REMOVE DURING IMPLEMENTATION
	return FALSE;
}

int __stdcall LZCNTInstructionHandler(
	char** instruction,		   // bytes of instruction
	CALLER_CONTEXT* context)   // caller context 
{
	UNREFERENCED_PARAMETER(context); // <-- LINE TO REMOVE DURING IMPLEMENTATION
	if (isLZCNT(*instruction)) {
		// Parse and emulate
		return TRUE;
	}
	else
		return FALSE; // not supported -> not handled
}
