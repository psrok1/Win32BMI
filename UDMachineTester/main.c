#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("ANDN: ");
	__try
	{
		_asm 
		{
			andn eax, eax, eax;
		}
		printf("supported\n");
	}
	__except (1)
	{
		printf("not supported\n");
	}

	printf("BEXTR: ");
	__try
	{
		_asm
		{
			bextr eax, eax, eax;
		}
		printf("supported\n");
	}
	__except (1)
	{
		printf("not supported\n");
	}

	printf("BLSI: ");
	__try
	{
		_asm
		{
			blsi eax, eax;
		}
		printf("supported\n");
	}
	__except (1)
	{
		printf("not supported\n");
	}

	printf("BLSMSK: ");
	__try
	{
		_asm
		{
			blsmsk eax, eax;
		}
		printf("supported\n");
	}
	__except (1)
	{
		printf("not supported\n");
	}

	printf("BLSR: ");
	__try
	{
		_asm
		{
			blsr eax, eax;
		}
		printf("supported\n");
	}
	__except (1)
	{
		printf("not supported\n");
	}

	printf("LZCNT: ");
	__try
	{
		_asm
		{
			lzcnt eax, eax;
		}
		printf("supported\n");
	}
	__except (1)
	{
		printf("not supported\n");
	}

	printf("POPCNT: ");
	__try
	{
		_asm
		{
			popcnt eax, eax;
		}
		printf("supported\n");
	}
	__except (1)
	{
		printf("not supported\n");
	}

	printf("TZCNT: ");
	__try
	{
		_asm
		{
			tzcnt eax, eax;
		}
		printf("supported\n");
	}
	__except (1)
	{
		printf("not supported\n");
	}

	system("pause");
}