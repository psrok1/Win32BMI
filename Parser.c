#include "Parser.h"
#include <stdio.h>

//
inline int getPrefix(unsigned char* instruction)
{

	//Check for 0x66 prefix
	if (instruction[0] == 0x66)
		return 1;
	return 0;
}

//sets the instr_args.type and returns the length of instruction opcode
int decodeInstructionType(unsigned char* instruction, int prefixOffset, ParsedInstruction* instr_args)
{
	//Check for VEX-coded instructions
	if ((instruction[0] & 0xFF) == 0xC4)
	{
		//second byte check - mandatory 11x on the front and 00010 on the back
		if ((instruction[1] & 0b11011111) != 0b11000010)
		{
			instr_args->type = INSTR_UNKNOWN;
			instr_args->length = 0;
			return 0;
		}
		//third byte check - mandatory 0xxxx0xx
		if ((instruction[2] & 0b10000100) != 0)
		{
			instr_args->type = INSTR_UNKNOWN;
			instr_args->length = 0;
			return 0;
		}

		//fourth byte check
		if ((instruction[3] & 0xFF) == 0xF2)
		{
			instr_args->type = INSTR_ANDN;
			return 4;
		}
		else if ((instruction[3] & 0xFF) == 0xF7)
		{
			instr_args->type = INSTR_BEXTR;
			return 4;
		}
		else if ((instruction[3] & 0xFF) == 0xF3)
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
		}
		else
		{
			instr_args->type = INSTR_UNKNOWN;
			instr_args->length = 0;
			return 0;
		}
	}

	//check for mandatory prefixes of non-vex instructions
	if ((instruction[0 + prefixOffset] & 0xFF) != 0xF3 || (instruction[1 + prefixOffset] & 0xFF) != 0x0F)
	{
		instr_args->type = INSTR_UNKNOWN;
		instr_args->length = 0;
		return 0;
	}

	//decode non-vex instruction type
	if ((instruction[2 + prefixOffset] & 0xFF) == 0xB8)
	{
		instr_args->type = INSTR_POPCNT;
		return 3;
	}
	else if ((instruction[2 + prefixOffset] & 0xFF) == 0xBC)
	{
		instr_args->type = INSTR_LZCNT;
		return 3;
	}
	else if ((instruction[2 + prefixOffset] & 0xFF) == 0xBD)
	{
		instr_args->type = INSTR_TZCNT;
		return 3;
	}

	instr_args->type = INSTR_UNKNOWN;
	return 0;
}

void decodeInstruction(unsigned char* instruction, int offset, int op16bit, ParsedInstruction* instr_args)
{
	unsigned char mod = instruction[offset] >> 6;
	unsigned char reg = (instruction[offset] & 0b00111000) >> 3;
	unsigned char rm = instruction[offset] & 0x07;
	unsigned char vvvv = 0;

	if (!(instr_args->type & INSTR_16BIT))
		op16bit = FALSE;

	if (instr_args->type & INSTR_VVVV_USED)
		vvvv = (~(instruction[offset - 2]) >> 3) & 0b00001111;

	if (instr_args->type & INSTR_DEST_REG)
		instr_args->dest = op16bit ? reg|0x10 : reg;

	if (instr_args->type & INSTR_DEST_VVVV)
		instr_args->dest = vvvv;

	if (instr_args->type & INSTR_SRC1_VVVV)
		instr_args->src1 = vvvv;

	if (instr_args->type & INSTR_SRC1_RM)
	{
		if (mod == 3)
			instr_args->src1 = op16bit ? rm | 0x10 : rm;
		else
			instr_args->src1 = op16bit ? MEM_16 : MEM_32;
	}

	if (instr_args->type & INSTR_SRC2_VVVV)
		instr_args->src2 = vvvv;

	if (instr_args->type & INSTR_SRC2_RM)
		instr_args->src2 = (mod == 3 ? rm : MEM_32);

	if (mod == 3)
	{
		instr_args->length = offset + 1;
		return;
	}
	// MOD = 0,1,2
	//check the SIB byte
	if (rm == 4)
	{
		++offset;
		unsigned char ss = instruction[offset] >> 6;
		unsigned char index = (instruction[offset] & 0b00111000) >> 3;
		unsigned char base = instruction[offset] & 0x07;
		instr_args->mem.scale = (1 << ss);

		if (index != 5)
			instr_args->mem.index = index;
		else
			instr_args->mem.index = UNDEF;

		if (base == 5 && mod == 0)
			instr_args->mem.base = UNDEF;
		else
			instr_args->mem.base = base;
	}
	//32 bit displacement
	if (mod == 2)
	{
		instr_args->mem.disp = *((int*)instruction[offset + 1]);
		if (rm != 4)
			instr_args->mem.index = rm;
		instr_args->length = offset + 5;
		return;
	}
	//8 bit displacement
	else if (mod == 1)
	{
		instr_args->mem.disp = instruction[offset + 1];
		if (rm != 4)
			instr_args->mem.index = rm;
		instr_args->length = offset + 2;
		return;
	}
	else // MOD == 0
	{
		//32 bit displacement-only
		if (rm == 5)
		{
			instr_args->mem.index = UNDEF;
			instr_args->mem.disp = *((int*)instruction[offset + 1]);
			instr_args->length = offset + 5;
			return;
		}
		else
		{
			instr_args->mem.index = rm;
			instr_args->length = offset;
			return;
		}
	}
}

ParsedInstruction parse(unsigned char* instruction)
{
	ParsedInstruction instr_args;
	instr_args.mem.base = UNDEF;
	instr_args.mem.disp = 0;
	instr_args.mem.index = UNDEF;
	instr_args.mem.scale = 1;
	int prefixOffset = 0, length = 0;

	prefixOffset = getPrefix(instruction);

	length += prefixOffset;

	length += decodeInstructionType(instruction, prefixOffset, &instr_args);

	if (instr_args.type == INSTR_UNKNOWN)
		return instr_args;

	decodeInstruction(instruction, length, prefixOffset, &instr_args);

	return instr_args;
}

void* getEffectiveVA(struct MemoryArgument mem, CALLER_CONTEXT* context) {
	int index = mem.index != UNDEF ? getRegValue(mem.index, context) : 0;
	int scale = mem.scale;
	int base = mem.base != UNDEF ? getRegValue(mem.base, context) : 0;
	int disp = mem.disp;
	return (void*)(index*scale + base + disp);
}