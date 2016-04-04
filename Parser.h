#pragma once
#include "Driver.h"
#include "Consts.h"

typedef struct {
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
} InstructionArgs;

extern InstructionArgs parse(
	char* instruction, 
	unsigned int type // INSTR_*
);

extern char* getOpcode(char* instruction);

extern void* getEffectiveVA(
	struct MemoryArgument mem,
	CALLER_CONTEXT* context);