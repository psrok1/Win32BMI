#include "Driver.h"
#include "Parser.h"

int __stdcall POPCNTInstructionEmulator(
	ParsedInstruction instruction,
	CALLER_CONTEXT* context)
{
	UNREFERENCED_PARAMETER(instruction);
	UNREFERENCED_PARAMETER(context);

	unsigned int src;
	if (instruction.src1 == MEM_32 || instruction.src1 == MEM_16)
	{
		src = *(unsigned int*)getEffectiveVA(instruction.mem, context);
	}
	else
	{
		src = getRegValue(instruction.src1, context);
	}

	unsigned int operand_size = 32;

	if (instruction.dest & 0x10) // 16-bit instruction
	{
		operand_size = 16;
	}

	unsigned int dest = 0;

	for (unsigned int i = 0; i < operand_size; i++)
	{
		if ((src >> i) & 0x1)
		{
			++dest;
		}
	}

	// Set flags
	context->flags &= (~FLAG_ZF) & (~FLAG_OF) & (~FLAG_SF) & (~FLAG_AF) & (~FLAG_CF) & (~FLAG_PF);
	if (src == 0)
	{
		context->flags |= FLAG_ZF;
	}

	setRegValue(instruction.dest, dest, context);

	return TRUE;
}