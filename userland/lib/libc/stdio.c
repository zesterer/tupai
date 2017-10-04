#include <stdio.h>

int puts(const char* str)
{
	asm volatile ("mov %0, %%eax; int $0x80" : "=r"(str) :: "%eax");
	return 0;
}
