#include "Driver.h"
#include "Parser.h"
#include "Emulators.h"

/* Main instruction handler */

int __stdcall HandleUndefInstruction(char** instruction, CALLER_CONTEXT* context)
{
	ParsedInstruction parsedInstruction = parse(*instruction);
	EmulatorRoutine routine;
	// Choose routine depending on the instruction type returned by parser
	switch (parsedInstruction.type)
	{
	case INSTR_ANDN:
		routine = EMULATOR_ROUTINE_PTR(ANDN);
		break;
	case INSTR_BEXTR:
		routine = EMULATOR_ROUTINE_PTR(BEXTR);
		break;
	case INSTR_BLSI:
		routine = EMULATOR_ROUTINE_PTR(BLSI);
		break;
	case INSTR_BLSMSK:
		routine = EMULATOR_ROUTINE_PTR(BLSMSK);
		break;
	case INSTR_BLSR:
		routine = EMULATOR_ROUTINE_PTR(BLSR);
		break;
	case INSTR_LZCNT:
		routine = EMULATOR_ROUTINE_PTR(LZCNT);
		break;
	case INSTR_POPCNT:
		routine = EMULATOR_ROUTINE_PTR(POPCNT);
		break;
	case INSTR_TZCNT:
		routine = EMULATOR_ROUTINE_PTR(TZCNT);
		break;
	case INSTR_DEBUG:
		routine = EMULATOR_ROUTINE_PTR(Example);
		break;
	default:
		// If instruction is unknown for parser: pass handling to O/S
		return FALSE;
	}
	// Call emulator routine.
	if (routine(parsedInstruction, context))
	{
		// On success: move instruction pointer and continue
		*instruction += parsedInstruction.length;
		return TRUE;
	}
	else
		// On failure: pass handling to O/S
		return FALSE;
}

/* Helper functions: context getters/setters */

unsigned int getRegValue(unsigned char src, CALLER_CONTEXT* context) 
{
	// Get pointer to first register value
	UINT32* reg_val = &context->eax;
	// Shift pointer to register specified as argument
	reg_val -= (src & 0x0F);
	// If source is 16b reg: return only 16 lsb
	// Otherwise: return whole value
	return (src & 0xF0) ? ((*reg_val) & 0xFFFF) : (*reg_val);
}

void setRegValue(unsigned char dst, unsigned int value, CALLER_CONTEXT* context) 
{
	// Get pointer to first register value
	UINT32* reg_val = &context->eax;
	// Shift pointer to register specified as argument
	reg_val -= (dst & 0x0F);
	// If source is 16b reg...
	if (dst & 0xF0)
		// ... merge 16 msb from origin and 16 lsb from argument
		*reg_val = (*reg_val & 0xFFFF0000) | (value & 0xFFFF);
	else
		// Otherwise: just overwrite register with specified value
		*reg_val = value;
}