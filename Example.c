#include "Driver.h"
#include "Parser.h"

/* Example! */

// Checking if opcode is associated with handled instruction
inline int isUD2(char* instruction) {
	return instruction[0] == 0x0F && instruction[1] == 0x0B;
}

int __stdcall UD2InstructionHandler(
	char** instruction,		   // bytes of instruction
	CALLER_CONTEXT* context)   // caller context 
{
	if (isUD2(*instruction)) {
		// Setting GPR registers!
		// Using getters
		setRegValue(REG_ECX, 0xFACEFEED, context);
		setRegValue(REG_EDX, 0x00000000, context);
		// ... or in structure
		context->ecx = 0xFACEFEED;
		context->edx = 0x00000000;
		context->flags |= FLAG_ZF;
		// Other registers (SSE), memory etc. need to be set directly!
		__asm movhlps xmm5, xmm4;
		// Move EIP two bytes forward! [instruction length]
		(*instruction) += 2;
		// Handled!
		return TRUE;
	}
	else
		return FALSE; // not UD2 -> not handled
}
