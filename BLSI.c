#include "Driver.h"
#include "Parser.h"

int __stdcall BLSIInstructionEmulator(
	ParsedInstruction instruction,
	CALLER_CONTEXT* context)
{
	UNREFERENCED_PARAMETER(instruction);
	UNREFERENCED_PARAMETER(context);

	return TRUE;
}