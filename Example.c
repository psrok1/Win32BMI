#include "Driver.h"
#include "Parser.h"

/* Example! */

int __stdcall ExampleInstructionEmulator(
	ParsedInstruction instruction,	// parsed instruction data
	CALLER_CONTEXT* context)		// caller context
{
	// Setting GPR registers!
	// Using setters
	setRegValue(REG_ECX, 0xFACEFEED, context);
	setRegValue(REG_EDX, 0x00000000, context);
	// .. or directly in structure
	context->ecx = 0xFACEFEED;
	context->edx = 0x00000000;
	context->flags |= FLAG_ZF;
	// Other registers (SSE), memory etc. need to be set directly!
	__asm movhlps xmm5, xmm4;
	// Memory? Use getEffectiveVA to get address from mem structure
	// But check earlier, whether you deal with 32b or 16b pointer!
	if(instruction.src1 == MEM_32)
		*((UINT32*)getEffectiveVA(instruction.mem, context)) = 0x00000000;
	// Success: handled!
	return TRUE;
}