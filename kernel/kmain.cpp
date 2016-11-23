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
#include <tupai/kprompt.hpp>
#include <tupai/mempool.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/gdt.hpp>
	#include <tupai/i686/idt.hpp>
	#include <tupai/i686/interrupt.hpp>
	#include <tupai/i686/kbd.hpp>
#endif

namespace tupai
{
	// Kernel early
	extern "C" void kearly()
	{
		tty_init();
		tty_write_str("[ OK ] Started kernel TTY\n");

		#if defined(SYSTEM_ARCH_i686)
			i686::gdt_init();
			tty_write_str("[ OK ] Initiated GDT\n");

			//i686::idt_init();
			//tty_write_str("[ OK ] Initiated IDT\n");

			//i686::interrupt_enable();
			//tty_write_str("[ OK ] Enabled interrupts\n");

			i686::kbd_init();
			tty_write_str("[ OK ] Initiated keyboard\n");
		#endif

		mempool_init((ubyte*)0x1000000, 0x100000, 64); // At 16 MB, 1 MB in size, composed of blocks of 64 B
		tty_write_str("[ OK ] Initiated dynamic memory pool\n");

		tty_write_str("[ OK ] Finished early kernel boot\n");
	}

	// Kernel main
	extern "C" void kmain()
	{
		tty_write_str("[ OK ] Main kernel boot complete\n");

		tty_write_str("\n");
		tty_write_str("Welcome to Tupai OS v0.1.0\n");
		tty_write_str("\n");

		while (kprompt() == 0);
	}
}
