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
	////////////////////////////////////////////////// ANDN 
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
	////////////////////////////////////////////////// BEXTR
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
	_asm {
		int 3h
		mov eax, 0xDEADBEEF
		mov ebx, 0xDEADBEEF
		mov ecx, 0xDEADBEEF
		mov edx, 0xDEADBEEF
		int 3h
	};
	////////////////////////////////////////////////// BLSI
	_asm {
		mov eax, 0xF0F0F0A8
		blsi ecx, eax	   	   // ecx = 0x08
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
		mov eax, 0xF0E00000
		blsi ecx, eax	   	   // ecx = 0x00200000
	}
	_asm {
		int 3h
		mov eax, 0xDEADBEEF
		mov ebx, 0xDEADBEEF
		mov ecx, 0xDEADBEEF
		mov edx, 0xDEADBEEF
		int 3h
	};
	////////////////////////////////////////////////// BLSMSK
	_asm {
		mov eax, 0xF0F0F0A8
		blsmsk ecx, eax	   	   // ecx = 0xF0F0F0AF
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
		mov eax, 0xF0E00000
		blsmsk ecx, eax	   	   // ecx = 0xF0FFFFFF
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
		mov eax, 0x00000001
		blsmsk ecx, eax	   	   // ecx = 0x00000001
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
		mov eax, 0x00000000
		blsmsk ecx, eax	   	   // ecx = 0xFFFFFFFF
	}
	_asm {
		int 3h
		mov eax, 0xDEADBEEF
		mov ebx, 0xDEADBEEF
		mov ecx, 0xDEADBEEF
		mov edx, 0xDEADBEEF
		int 3h
	};
	////////////////////////////////////////////////// BLSR
	_asm {
		mov eax, 0xF0F0F0A8
		blsr ecx, eax	   	   // ecx = 0xF0F0F0A0
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
		blsr ecx, eax	   	   // ecx = 0xFFFFFFFE
		int 3h
		blsr ecx, ecx	   	   // ecx = 0xFFFFFFFC
		int 3h
		blsr ecx, ecx	   	   // ecx = 0xFFFFFFF8
		int 3h
		blsr ecx, ecx	   	   // ecx = 0xFFFFFFF0
		int 3h
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
		mov eax, 0x00000000
		blsr ecx, ecx	   	   // ecx = 0x00000000
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
		mov eax, 0x00000000
		blsr ecx, ecx	   	   // ecx = 0x00000000
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
		mov eax, 0xFFFF0000
		lzcnt ecx, eax		  // 16
		int 3h
		mov eax, 0x0000FFFF
		lzcnt ecx, eax        // 0
		int 3h
		mov eax, 0x80808080
		lzcnt ecx, eax        // 7
		int 3h
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
		mov eax, 0xFFFF0000
		lzcnt cx, ax		  // 0
		int 3h
		mov eax, 0x0000FFFF
		lzcnt cx, ax          // 0
		int 3h
		mov eax, 0x80808080
		lzcnt cx, ax          // 7
		int 3h
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
		mov eax, 0xFFFF0000
		popcnt ecx, eax		   // 32
		int 3h
		mov eax, 0x0000FFFF
		popcnt ecx, eax        // 32
		int 3h
		mov eax, 0x80808080
		popcnt ecx, eax        // 4
		int 3h
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
		mov eax, 0xFFFF0000
		tzcnt ecx, eax		  // 0
		int 3h
		mov eax, 0x0000FFFF
		tzcnt ecx, eax        // 16
		int 3h
		mov eax, 0x40808080
		tzcnt ecx, eax        // 1
		int 3h
	}
	_asm {
		int 3h
		mov eax, 0xDEADBEEF
		mov ebx, 0xDEADBEEF
		mov ecx, 0xDEADBEEF
		mov edx, 0xDEADBEEF
		int 3h
	};
}