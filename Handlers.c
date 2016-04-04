#include "Driver.h"

/* External references to instruction emulators */

extern EMULATOR_ROUTINE(ANDNInstructionHandler);
extern EMULATOR_ROUTINE(BEXTRInstructionHandler);
extern EMULATOR_ROUTINE(BLSIInstructionHandler);
extern EMULATOR_ROUTINE(BLSMSKInstructionHandler);
extern EMULATOR_ROUTINE(BLSRInstructionHandler);
extern EMULATOR_ROUTINE(LZCNTInstructionHandler);
extern EMULATOR_ROUTINE(POPCNTInstructionHandler);
extern EMULATOR_ROUTINE(TZCNTInstructionHandler);

/* UD exception handlers chain */

InstructionHandler handlers_chain[] = {
	ANDNInstructionHandler,
	BEXTRInstructionHandler,
	BLSIInstructionHandler,
	BLSMSKInstructionHandler,
	BLSRInstructionHandler,
	LZCNTInstructionHandler,
	POPCNTInstructionHandler,
	TZCNTInstructionHandler,
	NULL // !!! last item must be NULL !!! [pass to system routine]
};

/* Helper functions: context getters/setters */

unsigned int getRegValue(unsigned char src, CALLER_CONTEXT* context) 
{
	UNREFERENCED_PARAMETER(src);
	UNREFERENCED_PARAMETER(context);
	// TODO
	return 0;
}

void setRegValue(unsigned char dst, unsigned int value, CALLER_CONTEXT* context) 
{
	UNREFERENCED_PARAMETER(dst);
	UNREFERENCED_PARAMETER(value);
	UNREFERENCED_PARAMETER(context);
	// TODO
}
