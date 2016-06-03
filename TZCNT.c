#include "Driver.h"
#include "Parser.h"

int __stdcall TZCNTInstructionEmulator(
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

	unsigned int temp = 0;
	unsigned int dest = 0;

	while ((temp < operand_size) && (((src >> temp) & 0x1) == 0))
	{
		--temp;
		++dest;
	}

	// Set flags
	context->flags &= (~FLAG_ZF) & (~FLAG_CF);
	if (dest == operand_size)
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