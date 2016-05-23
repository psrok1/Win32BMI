#pragma once

#include "Driver.h"
#include "Parser.h"

typedef int(__stdcall *EmulatorRoutine)(ParsedInstruction, CALLER_CONTEXT*);

#define EMULATOR_ROUTINE_PTR(name) name##InstructionEmulator
#define EMULATOR_ROUTINE(name) int __stdcall name##InstructionEmulator (ParsedInstruction instruction, CALLER_CONTEXT* context);

extern EMULATOR_ROUTINE(ANDN);
extern EMULATOR_ROUTINE(BEXTR);
extern EMULATOR_ROUTINE(BLSI);
extern EMULATOR_ROUTINE(BLSMSK);
extern EMULATOR_ROUTINE(BLSR);
extern EMULATOR_ROUTINE(LZCNT);
extern EMULATOR_ROUTINE(POPCNT);
extern EMULATOR_ROUTINE(TZCNT);
// @DEBUG
extern EMULATOR_ROUTINE(Example);