//
// file : kentry.cpp
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
#include <tupai/main.hpp>
#include <tupai/debug.hpp>
#include <tupai/arch.hpp>
#include <tupai/x86/multiboot.hpp>
#include <tupai/x86/amd64/gdt.hpp>
#include <tupai/x86/amd64/idt.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace x86
	{
		namespace amd64
		{
			static uint64_t mb_magic_tmp;
			static void* mb_header_tmp;

			extern "C" void kentry(uint64_t mb_magic, void* mb_header, void* stack)
			{
				// Save Multiboot data
				mb_magic_tmp = mb_magic;
				mb_header_tmp = mb_header;

				// Initiate debugging
				debug_init();

				// Passed information
				debug_print("Kernel virtual base is located at offset ", (void*)arch_get_offset(), '\n');
				debug_print( // Display kentry info
					"kentry at ", (void*)&kentry, " called with:\n",
					"  mb_magic  -> ", util::fmt_int<uint64_t>(mb_magic, 16), '\n',
					"  mb_header -> ", mb_header, '\n',
					"  stack     -> ", stack, '\n'
				);

				// Core system setup
				early();
			}

			extern "C" void kmain()
			{
				// Load multiboot information
				multiboot_set_header(mb_magic_tmp, mb_header_tmp);

				// Initiate and install the GDT
				gdt_init();
				gdt_install();

				// Initiate and install the IDT
				idt_init();
				idt_install();

				// x86 initiation
				arch_init();

				debug_print("Finished amd64 initiation\n");

				main(); // Main kernel entry
			}
		}
	}
}
