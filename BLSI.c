#include "Driver.h"
#include "Parser.h"

int __stdcall BLSIInstructionEmulator(
	ParsedInstruction instruction,
	CALLER_CONTEXT* context)
{
	UNREFERENCED_PARAMETER(instruction);
	UNREFERENCED_PARAMETER(context);

	unsigned int src;
	if (instruction.src1 == MEM_32)
	{
		src = *(unsigned int*)getEffectiveVA(instruction.mem, context);
	}
	else
	{
		src = getRegValue(instruction.src1, context);
	}

	unsigned int operand_size = 32;

	unsigned int dest = (-src) & src;

	// Set flags
	context->flags &= (~FLAG_ZF) & (~FLAG_SF) & (~FLAG_CF) & (~FLAG_OF);
	if (src == 0)
	{
		context->flags |= FLAG_CF;
	}

	if (dest == 0)
	{
		context->flags |= FLAG_ZF;
	}

	if (dest >> (operand_size - 1))
	{
		context->flags |= FLAG_SF;
	}

	setRegValue(instruction.dest, dest, context);

	return TRUE;
}