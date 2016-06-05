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

	unsigned int s_field;
	unsigned int l_field;

	if (start >= 32)
		s_field = 0;
	else
		s_field = (src1 >> start);

	if (len >= 32)
		l_field = 0;
	else
		l_field = (1 << len);

	unsigned int dest = s_field & (l_field - 1);

	// Set flags
	context->flags &= (~FLAG_ZF) & (~FLAG_CF) & (~FLAG_OF);

	if (dest == 0)
	{
		context->flags |= FLAG_ZF;
	}

	setRegValue(instruction.dest, dest, context);

	return TRUE;
}