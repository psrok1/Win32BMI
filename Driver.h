#pragma once
#include <ntddk.h>
#include <intrin.h>
#include "Consts.h"

#pragma pack(push,1)

#define INTR_MAX 256

// Single interrupt descriptor
typedef struct {
	UINT16 offs_lo; // offset bits 0..15
	UINT16 selector; // a code segment selector in GDT or LDT
	UINT8 zero;      // unused, set to 0
	UINT8 type_attr; // type and attributes
	UINT16 offs_hi; // offset bits 16..31
} IDTDescriptor;

// Interrupt Descriptor Table
typedef IDTDescriptor IDT[INTR_MAX], *PIDT;

// Content of 48-bit IDTR register
typedef struct
{
	UINT16 limit;
	PIDT   desc_table;
} IDTR;

// Interrupt Service Routine type
typedef int(__stdcall *ISR)();

typedef struct
{
	UINT32 edi;
	UINT32 esi;
	UINT32 ebp;
	UINT32 esp;
	UINT32 ebx;
	UINT32 edx;
	UINT32 ecx;
	UINT32 eax;
	UINT32 pseudoflags;
	UINT32 eip;
	UINT32 cs;
	UINT32 flags;
} CALLER_CONTEXT;

extern unsigned int getRegValue(
	unsigned char src,
	CALLER_CONTEXT* context);

extern void setRegValue(
	unsigned char dst,
	unsigned int value,
	CALLER_CONTEXT* context);

/*** Handlers.c ***/

extern int __stdcall HandleUndefInstruction(unsigned char** instruction, CALLER_CONTEXT* context);

#pragma pack(pop)
