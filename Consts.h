#pragma once

// 32b registers

#define REG_EAX 0x00
#define REG_ECX 0x01
#define REG_EDX 0x02
#define REG_EBX 0x03
#define REG_ESP 0x04
#define REG_EBP 0x05
#define REG_ESI 0x06
#define REG_EDI 0x07

// 16b registers

#define REG_AX  0x10
#define REG_CX  0x11
#define REG_DX  0x12
#define REG_BX  0x13
#define REG_SP  0x14
#define REG_BP  0x15
#define REG_SI  0x16
#define REG_DI  0x17

// Special values

#define MEM_16  0x20
#define MEM_32  0x21
#define UNDEF   0xFF

// Flags

#define FLAG_CF 0x0001	// Carry
#define FLAG_PF 0x0004	// Parity
#define FLAG_AF 0x0010  // Adjust
#define FLAG_ZF 0x0040  // Zero
#define FLAG_SF 0x0080  // Sign
#define FLAG_DF 0x0400  // Direction
#define FLAG_OF 0x0800  // Overflow

// Instruction descriptors

#define INSTR_ANDN		0x00000000
#define INSTR_BEXTR		0x10000000
#define INSTR_BLSI		0x20000000
#define INSTR_BLSMSK	0x30000000
#define INSTR_BLSR		0x40000000
#define INSTR_LZCNT		0x50000000
#define INSTR_POPCNT	0x60000000
#define INSTR_TZCNT		0x70000000
