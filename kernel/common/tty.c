//
// file : tty.c
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Tupai
#include <tupai/common/tty.h>

#if defined(ARCH_FAMILY_x86)
	#include <tupai/x86/textmode.h>
#elif defined(ARCH_FAMILY_arm)
	#include <tupai/arm/uart.h>
#else
	#warning "Architecture provides no TTY interface!"
#endif

// Standard
#include <stddef.h>
#include <stdint.h>

void tty_init()
{
	#if defined(ARCH_FAMILY_x86)
		textmode_init();
		textmode_clear();
	#elif defined(ARCH_FAMILY_arm)
		uart_init();
	#endif
}

void tty_print(const char* str)
{
	for (size_t i = 0; str[i] != '\0'; i ++)
	{
		#if defined(ARCH_FAMILY_x86)
			textmode_write(str[i]);
		#elif defined(ARCH_FAMILY_arm)
			uart_write(str[i]);
		#endif
	}
}
