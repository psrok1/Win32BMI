#pragma once
#include "Driver.h"
#include "Consts.h"

typedef struct {
	unsigned int type;
	
	unsigned char src1;
	unsigned char src2;
	unsigned char dest;

	unsigned int length; // instruction length in bytes

	// [base + index*scale + disp]
	// ex. MOV EAX, [ECX + EDX*2 + 100]
	struct MemoryArgument {
		unsigned char base;		// REG_* OR UNDEF
		unsigned char index;
		unsigned char scale;
		unsigned int  disp;		// VALUE
	} mem;
} ParsedInstruction;

extern int getPrefix(ParsedInstruction *instruction);

extern int decodeInstructionType(char* instruction, int prefixOffset, ParsedInstruction* instr_args);

extern void decodeAndn(char* instruction, int offset, ParsedInstruction* instr_args);

extern void decodeBextr(char* instruction, int offset, ParsedInstruction* instr_args);

extern void decodeBlsX(char* instruction, int offset, ParsedInstruction* instr_args);

extern void decodeXcnt(char* instruction, int offset, int op16bit, ParsedInstruction* instr_args);

extern ParsedInstruction parse(char* instruction);

extern void* getEffectiveVA(
	struct MemoryArgument mem,
	CALLER_CONTEXT* context);