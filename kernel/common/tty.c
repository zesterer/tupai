// Tupai
#include <tupai/common/tty.h>
#include <tupai/x86_family/textmode.h>

// Standard
#include <stddef.h>
#include <stdint.h>

void tty_init()
{
	textmode_init();
	textmode_clear();
}

void tty_print(const char* str)
{
	for (size_t i = 0; str[i] != '\0'; i ++)
		textmode_write(str[i]);
}
