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
#include <tupai/tty.hpp>
#include <tupai/debug.hpp>

#if defined(ARCH_FAMILY_x86)
	#include <tupai/x86/textmode.hpp>
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
		// Mirror with debugging interface
		debug_init();

		#if defined(ARCH_FAMILY_x86)
		{
			x86::textmode_init();
			x86::textmode_clear();
		}
		#endif
	}

	void tty_write(char c)
	{
		// Mirror TTY output with the debugging interface
		debug_write(c);

		#if defined(ARCH_FAMILY_x86)
			x86::textmode_write(x);
		#endif
	}

	void tty_print(const char* str)
	{
		for (size_t i = 0; str[i] != '\0'; i ++)
			tty_write(str[i]);
	}
}
