#include "Parser.h"

ParsedInstruction parse(char* instruction)
{
	ParsedInstruction instr_args;
	// *** During implementation: delete lines below ***
	if (instruction[0] == 0x0F && instruction[1] == 0x0B)
	{
		instr_args.type = INSTR_DEBUG;
		instr_args.src1 = UNDEF;
		instr_args.length = 2;
	}
	else
	{
		instr_args.type = INSTR_UNKNOWN;
		instr_args.length = 0;
	}
	// **** TODO ****
	return instr_args;
}

void* getEffectiveVA(struct MemoryArgument mem, CALLER_CONTEXT* context) {
	UNREFERENCED_PARAMETER(mem);
	UNREFERENCED_PARAMETER(context);
	return NULL;
}