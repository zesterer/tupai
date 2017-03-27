// Tupai
#include <tupai/common/tty.h>

#include <stdint.h>

void kmain()
{
	//uint16_t* test = (uint16_t*)0xB8000;
	//test[0] = 0x4F65;
	//return;

	tty_init();
	tty_print("Welcome to Tupai OS\nCopyright 2017, Joshua Barretto\n");
}
