/*
* 	file : kmain.cpp
*
* 	This file is part of Tupai.
*
* 	Tupai is free software: you can redistribute it and/or modify
* 	it under the terms of the GNU General Public License as published by
* 	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*
* 	Tupai is distributed in the hope that it will be useful,
* 	but WITHOUT ANY WARRANTY; without even the implied warranty of
* 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* 	GNU General Public License for more details.
*
* 	You should have received a copy of the GNU General Public License
* 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
*/

// Tupai
#include <tupai/kmain.hpp>
#include <tupai/kpanic.hpp>
#include <tupai/tty.hpp>
#include <tupai/prompt.hpp>
#include <tupai/mempool.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/gdt.hpp>
	#include <tupai/i686/idt.hpp>
	#include <tupai/i686/interrupt.hpp>
	#include <tupai/i686/kbd.hpp>
#endif

namespace tupai
{
	void kmain_write_check(const char* str, int error = 0)
	{
		tty_write('[');

		if (error == 0)
		{
			tty_set_fg_color(0x0A);
			tty_write_str(" OK ");
		}
		else
		{
			tty_set_fg_color(0x0C);
			tty_write_str("FAIL");
		}

		tty_set_fg_color(0x0F);
		tty_write_str("] ");
		tty_write_str(str);
		tty_write('\n');
	}

	// Kernel early
	extern "C" void kearly()
	{
		tty_init();
		kmain_write_check("Started kernel TTY");

		mempool_init((ubyte*)0x1000000, 0x100000, 64); // At 16 MB, 1 MB in size, composed of blocks of 64 B
		kmain_write_check("Initiated dynamic memory pool");

		#if defined(SYSTEM_ARCH_i686)
			i686::gdt_init();
			kmain_write_check("Initiated GDT");

			i686::idt_init();
			kmain_write_check("Initiated IDT");

			i686::interrupt_enable();
			kmain_write_check("Enabled interrupts");

			i686::kbd_init();
			kmain_write_check("Initiated keyboard");
		#endif

		kmain_write_check("Finished early kernel boot");
	}

	// Kernel main
	extern "C" void kmain()
	{
		kmain_write_check("Main kernel boot complete");

		tty_write_str("\n");
		tty_write_str("Welcome to Tupai OS v0.1.0\n");

		while (prompt() == 0);
	}
}
