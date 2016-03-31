#include "Driver.h"

int __stdcall UD2InstructionHandler(
	char** instruction,		   // bytes of instruction
	CALLER_CONTEXT* context)   // caller context 
{
	if ((*instruction)[0] == 0x0F && (*instruction)[1] == 0x0B) {
		context->ecx = 0xFACEFEED;  // ... ECX
		(*instruction) += 2;		// set EIP to next instruction
		return TRUE;				// handled!
	} else
		return FALSE; // not UD2 -> not handled
}

InstructionHandler handlers_chain[] = {
	UD2InstructionHandler,
	NULL // !!! last item must be NULL !!!
};
