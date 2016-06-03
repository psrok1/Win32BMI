#include "Driver.h"
#include "Parser.h"

int __stdcall BEXTRInstructionEmulator(
	ParsedInstruction instruction,
	CALLER_CONTEXT* context)
{
	UNREFERENCED_PARAMETER(instruction);
	UNREFERENCED_PARAMETER(context);

	unsigned int src1;
	if (instruction.src1 == MEM_32)
	{
		src1 = *(unsigned int*)getEffectiveVA(instruction.mem, context);
	}
	else
	{
		src1 = getRegValue(instruction.src1, context);
	}
	unsigned int src2 = getRegValue(instruction.src2, context);

	
	unsigned int start = src2 & 0xFF;
	unsigned int len = (src2 & 0xFF00) >> 8;

	unsigned int dest = (src1 >> start) & ((1 << len) - 1);

	// Set flags
	context->flags &= (~FLAG_ZF) & (~FLAG_CF) & (~FLAG_OF);

	if (dest == 0)
	{
		context->flags |= FLAG_ZF;
	}

	setRegValue(instruction.dest, dest, context);

	return TRUE;
}