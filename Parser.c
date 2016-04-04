#include "Parser.h"

InstructionArgs parse(char* instruction, unsigned int type)
{
	InstructionArgs instr_args;
	// *** During implementation: delete lines below ***
	instr_args.length = 0;
	UNREFERENCED_PARAMETER(instruction);
	UNREFERENCED_PARAMETER(type);
	// **** TODO ****
	return instr_args;
}

char* getOpcode(char* instruction) {
	UNREFERENCED_PARAMETER(instruction);
	// **** TODO ****
	return instruction;
}

void* getEffectiveVA(struct MemoryArgument mem, CALLER_CONTEXT* context) {
	UNREFERENCED_PARAMETER(mem);
	UNREFERENCED_PARAMETER(context);
	return NULL;
}