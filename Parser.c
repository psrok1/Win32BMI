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

void decodeAndn(char* instruction, int offset, ParsedInstruction* instr_args)
{
	unsigned char mod = instruction[offset] >> 6;
	unsigned char reg = (instruction[offset] & 0b00111000) >> 3;
	unsigned char rm = instruction[offset] & 0x07;
	unsigned char vvvv = (~(instruction[offset - 1]) >> 3) & 0b00001111;

	instr_args->dest = reg;
	instr_args->src1 = vvvv;

	//second src is register
	if (mod == 3)
	{
		instr_args->src2 = rm;
		instr_args->length = offset;
		return;
	}
	//second src is memory
	instr_args->src2 = MEM_32;
	//check the SIB byte
	if (rm == 5)
	{
		++offset;
		unsigned char ss = instruction[offset] >> 6;
		unsigned char index = (instruction[offset] & 0b00111000) >> 3;
		unsigned char base = instruction[offset] & 0x07;
		instr_args->mem.scale = pow(2, ss);
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
		instr_args->mem.disp = *((int*)instruction[offset+1]);
		if(rm!=5)
			instr_args->mem.index = rm;
		instr_args->length = offset + 4;
		return;
	}
	//8 bit displacement
	else if (mod == 1)
	{
		instr_args->mem.disp = instruction[offset+1];
		if (rm != 5)
			instr_args->mem.index = rm;
		instr_args->length = offset + 1;
		return;
	}
	else
	{
		//32 bit displacement without index
		if (rm == 6)
		{
			instr_args->mem.index = UNDEF;
			instr_args->mem.disp = *((int*)instruction[offset+1]);
			instr_args->length = offset + 4;
			return;
		}
		else
		{
			if (rm != 5)
				instr_args->mem.index = rm;
			instr_args->length = offset;
			return;
		}
	}
}

void decodeBextr(char* instruction, int offset, ParsedInstruction* instr_args)
{
	unsigned char mod = instruction[offset] >> 6;
	unsigned char reg = (instruction[offset] & 0b00111000) >> 3;
	unsigned char rm = instruction[offset] & 0x07;
	unsigned char vvvv = (~(instruction[offset - 1]) >> 3) & 0b00001111;

	instr_args->dest = reg;
	instr_args->src2 = vvvv;

	//first src is register
	if (mod == 3)
	{
		instr_args->src1 = rm;
		instr_args->length = offset;
		return;
	}
	//first src is memory
	instr_args->src1 = MEM_32;
	//check the SIB byte
	if (rm == 5)
	{
		++offset;
		unsigned char ss = instruction[offset] >> 6;
		unsigned char index = (instruction[offset] & 0b00111000) >> 3;
		unsigned char base = instruction[offset] & 0x07;
		instr_args->mem.scale = pow(2, ss);
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
		instr_args->mem.disp = *((int*)instruction[offset+1]);
		if (rm != 5)
			instr_args->mem.index = rm;
		instr_args->length = offset + 4;
		return;
	}
	//8 bit displacement
	else if (mod == 1)
	{
		instr_args->mem.disp = instruction[offset+1];
		if (rm != 5)
			instr_args->mem.index = rm;
		instr_args->length = offset + 1;
		return;
	}
	else
	{
		//32 bit displacement without index
		if (rm == 6)
		{
			instr_args->mem.index = UNDEF;
			instr_args->mem.disp = *((int*)instruction[offset+1]);
			instr_args->length = offset + 4;
			return;
		}
		else
		{
			if (rm != 5)
				instr_args->mem.index = rm;
			instr_args->length = offset;
			return;
		}
	}
}

void decodeBlsX(char* instruction, int offset, ParsedInstruction* instr_args)
{
	unsigned char mod = instruction[offset] >> 6;
	unsigned char rm = instruction[offset] & 0x07;
	unsigned char vvvv = (~(instruction[offset - 1]) >> 3) & 0b00001111;

	instr_args->dest = vvvv;

	//first src is register
	if (mod == 3)
	{
		instr_args->src1 = rm;
		instr_args->length = offset;
		return;
	}
	//first src is memory
	instr_args->src1 = MEM_32;
	//check the SIB byte
	if (rm == 5)
	{
		++offset;
		unsigned char ss = instruction[offset] >> 6;
		unsigned char index = (instruction[offset] & 0b00111000) >> 3;
		unsigned char base = instruction[offset] & 0x07;
		instr_args->mem.scale = pow(2, ss);
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
		instr_args->mem.disp = *((int*)instruction[offset+1]);
		if (rm != 5)
			instr_args->mem.index = rm;
		instr_args->length = offset + 4;
		return;
	}
	//8 bit displacement
	else if (mod == 1)
	{
		instr_args->mem.disp = instruction[offset+1];
		if (rm != 5)
			instr_args->mem.index = rm;
		instr_args->length = offset + 1;
		return;
	}
	else
	{
		//32 bit displacement without index
		if (rm == 6)
		{
			instr_args->mem.index = UNDEF;
			instr_args->mem.disp = *((int*)instruction[offset+1]);
			instr_args->length = offset + 4;
			return;
		}
		else
		{
			if (rm != 5)
				instr_args->mem.index = rm;
			instr_args->length = offset;
			return;
		}
	}
}

void decodeXcnt(char* instruction, int offset, int op16bit, ParsedInstruction* instr_args)
{
	unsigned char mod = instruction[offset] >> 6;
	unsigned char reg = (instruction[offset] & 0b00111000) >> 3;
	unsigned char rm = instruction[offset] & 0x07;

	//adjust the length of registers
	if (op16bit)
	{
		instr_args->src1 = MEM_16;
		reg |= 0x10;
	}
	else
		instr_args->src1 = MEM_32;

	instr_args->dest = reg;

	//first src is register
	if (mod == 3)
	{
		if (op16bit)
			instr_args->src1 = rm | 0x10;
		else
			instr_args->src1 = rm;
		instr_args->length = offset;
		return;
	}

	//check the SIB byte
	if (rm == 5)
	{
		++offset;
		unsigned char ss = instruction[offset] >> 6;
		unsigned char index = (instruction[offset] & 0b00111000) >> 3;
		unsigned char base = instruction[offset] & 0x07;
		instr_args->mem.scale = pow(2, ss);
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
		instr_args->mem.disp = *((int*)instruction[offset+1]);
		if (rm != 5)
			instr_args->mem.index = rm;
		instr_args->length = offset + 4;
		return;
	}
	//8 bit displacement
	else if (mod == 1)
	{
		instr_args->mem.disp = instruction[offset+1];
		if (rm != 5)
			instr_args->mem.index = rm;
		instr_args->length = offset + 1;
		return;
	}
	else
	{
		//32 bit displacement without index
		if (rm == 6)
		{
			instr_args->mem.index = UNDEF;
			instr_args->mem.disp = *((int*)instruction[offset+1]);
			instr_args->length = offset + 4;
			return;
		}
		else
		{
			if (rm != 5)
				instr_args->mem.index = rm;
			instr_args->length = offset;
			return;
		}
	}
}

ParsedInstruction parse(char* instruction)
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

	if (instr_args.type==INSTR_UNKNOWN)
		return instr_args;

	switch (instr_args.type)
	{
	case INSTR_ANDN: decodeAndn(instruction, length, &instr_args); break;
	case INSTR_BEXTR: decodeBextr(instruction, length, &instr_args); break;
	case INSTR_BLSI: decodeBlsX(instruction, length, &instr_args); break;
	case INSTR_BLSMSK: decodeBlsX(instruction, length, &instr_args); break;
	case INSTR_BLSR: decodeBlsX(instruction, length, &instr_args); break;
	case INSTR_LZCNT: decodeXcnt(instruction, length, prefixOffset, &instr_args); break;
	case INSTR_POPCNT: decodeXcnt(instruction, length, prefixOffset, &instr_args); break;
	case INSTR_TZCNT: decodeXcnt(instruction, length, prefixOffset, &instr_args); break;
	}


	return instr_args;
}

void* getEffectiveVA(struct MemoryArgument mem, CALLER_CONTEXT* context) {
	int index = mem.index != UNDEF ? getRegValue(mem.index, context) : 0;
	int scale = mem.scale;
	int base = mem.base != UNDEF ? getRegValue(mem.base, context) : 0;
	int disp = mem.disp;
	return index*scale + base + disp;
}