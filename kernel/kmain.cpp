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
#include <tupai/tty.hpp>
#include <tupai/prompt.hpp>
#include <tupai/mempool.hpp>
#include <tupai/syscall.hpp>
#include <tupai/task.hpp>
#include <tupai/console.hpp>
#include <tupai/kdebug.hpp>

// --- EARLY ---
#include <tupai/startup.hpp>
#include <tupai/early/out.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/gdt.hpp>
	#include <tupai/i686/idt.hpp>
	#include <tupai/i686/interrupt.hpp>
	#include <tupai/i686/kbd.hpp>
	#include <tupai/i686/pit.hpp>
	#include <tupai/i686/paging.hpp>
	#include <tupai/i686/serial.hpp>

	#include <tupai/x86_family/multiboot.hpp>
	#include <tupai/x86_family/vga.hpp>
#endif

// --- TESTING ---

namespace tupai
{
	// Kernel early
	void kearly(ptr_t mb_header, uint32 mb_magic, uint32 stack)
	{
		#if defined(SYSTEM_ARCH_i686)
			// Multiboot
			x86_family::multiboot_init(mb_header, mb_magic);

			// Serial debug
			#if defined(CFG_ENABLE_SERIAL_DEBUG)
				serial_init();
				startup_print_unit_init("Serial");
				serial_open(1);
				startup_print_unit_init("COM1 debugging");
			#endif

			// Paging
			paging_init();
			startup_print_unit_init("Paging structure");
			paging_enable();
			startup_print_unit_init("Paging");

			// Dynamic memory pool
			mempool_init((ubyte*)mempool_begin, mempool_size, 64); // Blocks of 64B
			startup_print_unit_init("Dynamic memory pool");

			// VGA
			x86_family::vga_init();
			startup_print_unit_init("VGA driver");

			// Console
			console_init_global();
			startup_print_unit_init("Kernel console");

			// GDT
			gdt_init();
			startup_print_unit_init("GDT");

			// IDT
			idt_init();
			startup_print_unit_init("IDT");

			// PIT
			pit_init();
			startup_print_unit_init("PIT");

			// Keyboard
			kbd_init();
			startup_print_unit_init("Keyboard");

			// Serial
			serial_init();
			startup_print_unit_init("Serial");
		#endif

		syscall_init();
		startup_print_unit_init("Syscall routine");

		task_init();
		startup_print_unit_init("Task scheduler");

		early::print("Boot procedure complete.\n");
	}

	// Kernel main
	void kmain()
	{
		interrupt_enable();
		pit_enable();
		kbd_enable();

		startup_welcome();

		while (prompt() == 0);
	}
}
