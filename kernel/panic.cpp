//
// file : panic.cpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Tupai
#include <tupai/panic.hpp>
#include <tupai/cpu.hpp>
#include <tupai/irq.hpp>

#include <tupai/util/fmt.hpp>
#if defined(ARCH_FAMILY_x86)
	#include <tupai/x86/textmode.hpp>
#endif

namespace tupai
{
	void panic(const char* msg)
	{
		irq::disable();

		char str[1024];

		util::fmt(str,
		 	"Kernel Panic!\n\n",
			"Something went badly wrong and the following error was produced.\n\n",
			msg, "\n\n",
			"Please send this error and all other relevant details concerning\nthis crash to ", P_MAINTAINER_EMAIL, '.'
		);

		#if defined(ARCH_FAMILY_x86)
			x86::textmode_clear();
			x86::textmode_move(0, 0);
			for (size_t i = 0; str[i] != '\0'; i ++)
				x86::textmode_write(str[i]);
		#endif

		cpu::hang();
	}
}
