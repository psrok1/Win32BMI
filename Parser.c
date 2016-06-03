#include "Parser.h"

//
inline int getPrefix(char* instruction)
{

	//Check for 0x66 prefix
	if (instruction[0] == 0x66)
		return 1;
	return 0;
}

//sets the instr_args.type and returns the length of instruction opcode
int decodeInstructionType(char* instruction, int prefixOffset, ParsedInstruction* instr_args)
{
	//Check for VEX-coded instructions
	if (instruction[0] == 0xC4)
	{
		//second byte check - mandatory 11x on the front and 00010 on the back
		if ((instruction[1] & 0b11011111) != 0b11000010)
		{
			instr_args->type = INSTR_UNKNOWN;
			instr_args->length = 0;
			return 0;
		}
		//third byte check - mandatory 0xxxx0xx
		if ((instruction[2]) != 0)
		{
			instr_args->type = INSTR_UNKNOWN;
			instr_args->length = 0;
			return 0;
		}

		//fourth byte check
		if (instruction[3] == 0xF2)
		{
			instr_args->type = INSTR_ANDN;
			return 4;
		}
		else if (instruction[3] == 0xF7)
		{
			instr_args->type = INSTR_BEXTR;
			return 4;
		}
		else if (instruction[3] == 0xF3)
		{
			//check reg value of ModR/M
			if ((instruction[4] & 0b00111000) >> 3 == 3)
			{
				instr_args->type = INSTR_BLSI;
				return 4;
			}
			else if ((instruction[4] & 0b00111000) >> 3 == 2)
			{
				instr_args->type = INSTR_BLSMSK;
				return 4;
			}
			else if ((instruction[4] & 0b00111000) >> 3 == 1)
			{
				instr_args->type = INSTR_BLSR;
				return 4;
			}
			else
			{
				instr_args->type = INSTR_UNKNOWN;
				instr_args->length = 0;
				return 0;
			}

			//
		}
		else
		{
			instr_args->type = INSTR_UNKNOWN;
			instr_args->length = 0;
			return 0;
		}
	}

	//check for mandatory prefixes of non-vex instructions
	if (instruction[0 + prefixOffset] != 0xF3 || instruction[1 + prefixOffset] != 0x0F)
	{
		instr_args->type = INSTR_UNKNOWN;
		instr_args->length = 0;
		return instr_args;
	}

	//decode non-vex instruction type
	if (instruction[2 + prefixOffset] == 0xB8)
	{
		instr_args->type = INSTR_POPCNT;
		return 3;
	}
	else if (instruction[2 + prefixOffset] == 0xBC)
	{
		instr_args->type = INSTR_LZCNT;
		return 3;
	}
	else if (instruction[2 + prefixOffset] == 0xBD)
	{
		instr_args->type = INSTR_TZCNT;
		return 3;
	}

	instr_args->type = INSTR_UNKNOWN;
	return 0;
}

void decodeOperands(char* instruction, int offset, int op16bit, ParsedInstruction* instr_args)
{
	unsigned char mod = instruction[offset]>>6;
	unsigned char reg = (instruction[offset] & 0b00111000) >> 3;
	unsigned char rm = instruction[offset] & 0x07;
	unsigned char vvvv = (~(instruction[offset - 1]) >> 3) & 0b00001111;

	if (instr_args->type == INSTR_POPCNT || instr_args->type == INSTR_LZCNT || instr_args->type == INSTR_TZCNT || instr_args->type == INSTR_BEXTR || instr_args == INSTR_ANDN)
	{
		if (op16bit)
			instr_args->dest = reg & 0x10;
		else
			instr_args->dest = reg;
	}
	else
	{
		instr_args->dest = vvvv;
	}

	if (instr_args->type == INSTR_ANDN)
		instr_args->src1 = vvvv;
	else if (mod == 3)
	{
		if (op16bit)
			instr_args->src1 = rm & 0x10;
		else
			instr_args->src1 = rm;
	}

	//TBC - pewnie skonczy sie na refaktoryzacji do postaci funkcji dekodujacych operandy dla kazdej instrukcji oddzielnie
}

ParsedInstruction parse(char* instruction)
{
	ParsedInstruction instr_args;
	int prefixOffset = 0, length = 0;

	prefixOffset = getPrefixes(instruction);

	length += prefixOffset;

	length += decodeInstructionType(instruction, prefixOffset, &instr_args);

	if (instr_args.type==INSTR_UNKNOWN)
		return instr_args;

	decodeOperands(instruction, length, prefixOffset, &instr_args);

	return instr_args;
}

void* getEffectiveVA(struct MemoryArgument mem, CALLER_CONTEXT* context) {
	UNREFERENCED_PARAMETER(mem);
	UNREFERENCED_PARAMETER(context);
	return NULL;
}