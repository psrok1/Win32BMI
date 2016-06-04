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

#define INSTR_VVVV_USED 0x0001
#define INSTR_16BIT     0x0002
#define INSTR_SRC1_VVVV 0x0010
#define INSTR_SRC1_RM   0x0020
#define INSTR_SRC2_VVVV 0x0100
#define INSTR_SRC2_RM   0x0200
#define INSTR_DEST_VVVV 0x1000
#define INSTR_DEST_REG  0x2000


#define INSTR_ANDN	    (0x00000000 | INSTR_VVVV_USED | INSTR_SRC1_VVVV | INSTR_SRC2_RM   | INSTR_DEST_REG)
#define INSTR_BEXTR	    (0x10000000 | INSTR_VVVV_USED | INSTR_SRC1_RM   | INSTR_SRC2_VVVV | INSTR_DEST_REG)
#define INSTR_BLSI	    (0x20000000 | INSTR_VVVV_USED | INSTR_SRC1_RM                     | INSTR_DEST_VVVV)
#define INSTR_BLSMSK    (0x30000000 | INSTR_VVVV_USED | INSTR_SRC1_RM                     | INSTR_DEST_VVVV)
#define INSTR_BLSR	    (0x40000000 | INSTR_VVVV_USED | INSTR_SRC1_RM                     | INSTR_DEST_VVVV)
#define INSTR_LZCNT	    (0x50000000 | INSTR_16BIT     | INSTR_SRC1_RM                     | INSTR_DEST_REG)
#define INSTR_POPCNT    (0x60000000 | INSTR_16BIT     | INSTR_SRC1_RM                     | INSTR_DEST_REG)
#define INSTR_TZCNT	    (0x70000000 | INSTR_16BIT     | INSTR_SRC1_RM                     | INSTR_DEST_REG)
#define INSTR_DEBUG     (0xE0000000)
#define INSTR_UNKNOWN   (0xF0000000)