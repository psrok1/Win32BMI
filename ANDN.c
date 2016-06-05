#include "Driver.h"
#include "Parser.h"

int __stdcall ANDNInstructionEmulator(
	ParsedInstruction instruction,
	CALLER_CONTEXT* context)
{
	UNREFERENCED_PARAMETER(instruction);
	UNREFERENCED_PARAMETER(context);
	
	unsigned int src1 = getRegValue(instruction.src1, context);
	unsigned int src2;
	if (instruction.src2 == MEM_32)
	{
		src2 = *(unsigned int*)getEffectiveVA(instruction.mem, context);
	}
	else
	{
		src2 = getRegValue(instruction.src2, context);
	}

	unsigned int dest = (~src1) & src2;
	unsigned int SF = dest >> 31;

	// Set flags
	context->flags &= (~FLAG_SF) & (~FLAG_ZF) & (~FLAG_OF) & (~FLAG_CF);
	if (SF)
	{
		context->flags |= FLAG_SF;
	}

	if (dest == 0)
	{
		context->flags |= FLAG_ZF;
	}

	setRegValue(instruction.dest, dest, context);

	return TRUE;
}