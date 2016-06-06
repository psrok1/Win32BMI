#include <stdio.h>

char data_block[256];

void cleanup()
{
	_asm {
		mov eax, 0xDEADBEEF
		mov ebx, 0xDEADBEEF
		mov ecx, 0xDEADBEEF
		mov edx, 0xDEADBEEF
	};
}

int main()
{
	// INITIALIZER
	*((unsigned int*)data_block) = 0xF0F0F0F0;
	*((unsigned int*)(&data_block[4])) = 0x84848484;
	
	////////////////////////////////////////////////// ANDN 
	
	cleanup();
	_asm {
		mov eax, 0x0F0F0F0F;
		or al, 0xff;
		sub al, 0x80;
		stc;
		int 3h;
		andn ecx, eax, eax			// 0x00000000 SF- ZF+
		int 3h;
	}
	
	cleanup();
	_asm {
		mov eax, 0x0F0F0F0F
		int 3h;
		andn ecx, eax, data_block	// 0xF0F0F0F0 SF+ ZF-
		int 3h;
	}
	
	cleanup();
	_asm {
		mov edx, 2
		mov ebx, 1
		mov eax, 0x0000FFFF
		int 3h;
		andn ecx, eax, data_block[edx+ebx*2] // 84840000
		int 3h;
	}
	
	////////////////////////////////////////////////// BEXTR
	cleanup();
	_asm {
		mov eax, 0xFFFFFFFF
		mov ecx, 0xFFFFFF80
		mov edx, 0x0503			// from 3 to 7
		int 3h;
		bextr ecx, eax, edx		// cl = 0x1F
		int 3h;
	}

	cleanup();
	_asm {
		mov eax, 0xCC88CC88
		mov ecx, 0xFFFF88FF
		mov edx, 0x0818			// from 24 to 31
		int 3h;
		bextr ecx, eax, edx		// cl = 0xCC
		int 3h;
	}

	cleanup();
	_asm {
		mov eax, 0xCC88CC88
		mov edx, 0xFFFF			// out of range
		mov ecx, 0xFFFFFFFF
		int 3h;
		bextr ecx, eax, edx		// ZF+
		int 3h;
	}

	////////////////////////////////////////////////// BLSI

	cleanup();
	_asm {
		mov eax, 0xF0F0F0A8
		int 3h;
		blsi ecx, eax	   	   // ecx = 0x08	CF+
		int 3h;
	}

	cleanup();
	_asm {
		mov eax, 0xF0E00000
		int 3h;
		blsi ecx, eax	   	   // ecx = 0x00200000
		int 3h;
	}

	cleanup();
	_asm {
		xor eax, eax
		int 3h;
		blsi ecx, eax	   	   // ecx = 0x00000000	CF- ZF+
		int 3h;
	}

	////////////////////////////////////////////////// BLSMSK
	cleanup();
	_asm {
		mov eax, 0xF0F0F0A8
		int 3h;
		blsmsk ecx, eax	   	   // ecx = 0x0000000F
		int 3h;
	}

	cleanup();
	_asm {
		mov eax, 0xF0E00000
		int 3h;
		blsmsk ecx, eax	   	   // ecx = 0x003FFFFF
		int 3h;
	}

	cleanup();
	_asm {
		mov eax, 0x00000001
		int 3h;
		blsmsk ecx, eax	   	   // ecx = 0x00000001
		int 3h;
	}

	cleanup();
	_asm {
		mov eax, 0x00000000
		int 3h;
		blsmsk ecx, eax	   	   // ecx = 0xFFFFFFFF
		int 3h;
	}
	////////////////////////////////////////////////// BLSR
	cleanup();
	_asm {
		mov eax, 0xF0F0F0A8
		int 3h;
		blsr ecx, eax	   	   // ecx = 0xF0F0F0A0
		int 3h;
	}
	
	cleanup();
	_asm {
		mov eax, 0xFFFFFFFF
		blsr ecx, eax	   	   // ecx = 0xFFFFFFFE
		int 3h
		blsr ecx, ecx	   	   // ecx = 0xFFFFFFFC
		int 3h
		blsr ecx, ecx	   	   // ecx = 0xFFFFFFF8
		int 3h
		blsr ecx, ecx	   	   // ecx = 0xFFFFFFF0
		int 3h
	}
}