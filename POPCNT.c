#include "Driver.h"
#include "Parser.h"

inline int isPOPCNT(char* instruction) {
	UNREFERENCED_PARAMETER(instruction); // <-- LINE TO REMOVE DURING IMPLEMENTATION
	return FALSE;
}

int __stdcall POPCNTInstructionHandler(
	char** instruction,		   // bytes of instruction
	CALLER_CONTEXT* context)   // caller context 
{
	UNREFERENCED_PARAMETER(context); // <-- LINE TO REMOVE DURING IMPLEMENTATION
	if (isPOPCNT(*instruction)) {
		// Parse and emulate
		return TRUE;
	}
	else
		return FALSE; // not supported -> not handled
}
