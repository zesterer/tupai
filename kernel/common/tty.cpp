//
// file : tty.cpp
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
#include <tupai/common/tty.hpp>

#if defined(ARCH_FAMILY_x86)
	#include <tupai/x86/textmode.hpp>
#elif defined(ARCH_rpi2)
	#include <tupai/arm/rpi2/uart.hpp>
#else
	#warning "Architecture provides no TTY interface!"
#endif

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	void tty_init()
	{
		#if defined(ARCH_FAMILY_x86)
			x86::textmode_init();
			x86::textmode_clear();
		#elif defined(ARCH_rpi2)
			arm::rpi2::uart_init();
		#endif
	}

	void tty_print(const char* str)
	{
		for (size_t i = 0; str[i] != '\0'; i ++)
		{
			#if defined(ARCH_FAMILY_x86)
				x86::textmode_write(str[i]);
			#elif defined(ARCH_rpi2)
				arm::rpi2::uart_write(str[i]);
			#endif
		}
	}
}
