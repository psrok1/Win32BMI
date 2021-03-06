#include "Driver.h"
#include "Parser.h"

int __stdcall LZCNTInstructionEmulator(
	ParsedInstruction instruction,
	CALLER_CONTEXT* context)
{
	UNREFERENCED_PARAMETER(instruction);
	UNREFERENCED_PARAMETER(context);

	unsigned int src;
	if (instruction.src1 == MEM_32 || instruction.src1 == MEM_16 )
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

	int temp = operand_size - 1;
	int dest = 0;

	while ((temp >= 0) && (((src >> temp) & 1) == 0))
	{
		--temp;
		++dest;
	}

	// Set flags
	context->flags &= (~FLAG_ZF) & (~FLAG_CF);
	if (dest == (int)operand_size)
	{
		context->flags |= FLAG_CF;
	}

	if (dest == 0)
	{
		context->flags |= FLAG_ZF;
	}

	setRegValue(instruction.dest, dest, context);

	return TRUE;
}