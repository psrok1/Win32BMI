#include <stdio.h>

char data_block[256];

int main()
{
	// INITIALIZER
	*((unsigned int*)data_block) = 0xF0F0F0F0;
	*((unsigned int*)(&data_block[4])) = 0x84848484;
	_asm { 
		int 3h 
		mov eax, 0xDEADBEEF 
		mov ebx, 0xDEADBEEF 
		mov ecx, 0xDEADBEEF 
		mov edx, 0xDEADBEEF 
		int 3h 
	};
	// ANDN 
	_asm {
		mov eax, 0x0F0F0F0F
		andn ecx, eax, eax			// 0x00000000
	}
	_asm {
		int 3h
		mov eax, 0xDEADBEEF
		mov ebx, 0xDEADBEEF
		mov ecx, 0xDEADBEEF
		mov edx, 0xDEADBEEF
		int 3h
	}
	_asm {
		mov eax, 0x0F0F0F0F
		andn ecx, eax, data_block	// 0xF0F0F0F0
	}
	_asm {
		int 3h
		mov eax, 0xDEADBEEF
		mov ebx, 0xDEADBEEF
		mov ecx, 0xDEADBEEF
		mov edx, 0xDEADBEEF
		int 3h
	};	
	_asm {
		mov edx, 2
		mov ebx, 1
		mov eax, 0x0000FFFF
		andn ecx, eax, data_block[edx+ebx*2] // 84840000
	}
	_asm {
		int 3h
		mov eax, 0xDEADBEEF
		mov ebx, 0xDEADBEEF
		mov ecx, 0xDEADBEEF
		mov edx, 0xDEADBEEF
		int 3h
	};
	_asm {
		mov eax, 0xFFFFFFFF
		mov ecx, 0xFFFFFF80
		mov edx, 0x0503			// from 3 to 7
		bextr ecx, eax, edx		// cl = 0x1F
	}
	_asm {
		int 3h
		mov eax, 0xDEADBEEF
		mov ebx, 0xDEADBEEF
		mov ecx, 0xDEADBEEF
		mov edx, 0xDEADBEEF
		int 3h
	};
	_asm {
		mov eax, 0xCC88CC88
		mov ecx, 0xFFFF88FF
		mov edx, 0x0818			// from 24 to 31
		bextr ecx, eax, edx		// cl = 0xCC
	}
	_asm {
		int 3h
		mov eax, 0xDEADBEEF
		mov ebx, 0xDEADBEEF
		mov ecx, 0xDEADBEEF
		mov edx, 0xDEADBEEF
		int 3h
	};
	_asm {
		mov eax, 0xCC88CC88
		mov edx, 0xFFFF			// from 24 to 31
		mov ecx, 0xFFFFFFFF
		bextr ecx, eax, edx		// ecx = 0x0
	}
}