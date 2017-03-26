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

#include <tupai/util/conv.hpp>
#include <tupai/util/out.hpp>

// --- MEMORY ---
#include <tupai/mempool.hpp>
#include <tupai/memory.hpp>

// --- SYSTEM INTERFACES ---
#include <tupai/sys/random.hpp>

// --- MULTITASKING ---
#include <tupai/syscall.hpp>
#include <tupai/task.hpp>

// --- CONSOLE & I/O ---
#include <tupai/tty.hpp>
#include <tupai/kshell.hpp>

// --- FILESYSTEM ---
#include <tupai/fs/fs.hpp>

// --- DEBUGGING ---
#include <tupai/kdebug.hpp>

// --- EARLY ---
#include <tupai/startup.hpp>

#if defined(SYSTEM_ARCH_i686)
	// --- ESSENTIAL X86 THINGS ---
	#include <tupai/i686/gdt.hpp>
	#include <tupai/i686/idt.hpp>
	#include <tupai/i686/interrupt.hpp>
	#include <tupai/i686/pit.hpp>

	// --- X86 MEMORY ---
	#include <tupai/i686/paging.hpp>

	// --- X86 I/O AND DEVICES ---
	#include <tupai/i686/kbd.hpp>
	#include <tupai/i686/serial.hpp>
	#include <tupai/x86_family/vga.hpp>

	// --- X86 MULTIBOOT ---
	#include <tupai/x86_family/multiboot.hpp>
#endif

// --- TESTING ---

namespace tupai
{
	static void kernel_shell_task();

	// Kernel early
	void kearly(ptr_t mb_header, uint32 mb_magic, uint32 stack __attribute__ ((unused)))
	{
		#if defined(SYSTEM_ARCH_i686)
			// Multiboot
			x86_family::multiboot_init(mb_header, mb_magic);

			// Serial debug
			#if defined(CFG_ENABLE_SERIAL_DEBUG)
				serial_init();
				startup_print_unit_init("serial");
				serial_open(1);
				startup_print_unit_init("COM1 debugging");
			#endif
		#endif

		// Memory management unit
		memory_init();
		startup_print_unit_init("memory");
		memory_enforce();
		startup_print_unit_init("memory enforcement");

		#if defined(SYSTEM_ARCH_i686)
			// Paging
			paging_init();
			startup_print_unit_init("paging structure");
			paging_enable();
			startup_print_unit_init("paging");

			// Dynamic memory pool
			mempool_init((ubyte*)mempool_begin, mempool_size, 1024); // Blocks of 1K
			startup_print_unit_init("dynamic memory pool");

			// GDT
			gdt_init();
			startup_print_unit_init("GDT");

			// IDT
			idt_init();
			startup_print_unit_init("IDT");
		#endif
	}

	// Kernel main
	void kmain()
	{
		// Enable graphical drivers
		#if defined(SYSTEM_ARCH_i686)
			// VGA
			x86_family::vga_init();
			startup_print_unit_init("VGA driver");
		#endif

		// TTY
		tty_init();
		startup_print_unit_init("TTY");

		// Now we have a TTY up and ready!

		#if defined(SYSTEM_ARCH_i686)
			// PIT
			pit_init();
			startup_print_unit_init("timer");

			// Keyboard
			kbd_init();
			startup_print_unit_init("keyboard");

			// Serial
			serial_init();
			startup_print_unit_init("serial");
		#endif

		// Syscall procedure
		syscall_init();
		startup_print_unit_init("syscall routine");

		// Kernel multi-tasking
		task_init();
		startup_print_unit_init("task scheduler");

		// Filesystems and I/O
		fs::fs_init();
		startup_print_unit_init("filesystem");

		// System interfaces
		sys::random_init();

		// Enable various systems
		interrupt_enable();
		pit_enable();
		kbd_enable();

		// Get CR3
		uint32 cr3;
		asm volatile("movl %%cr3, %%eax; movl %%eax, %0":"=m"(cr3)::"%eax"); // Get cr3

		// Kernel tasks
		task_add("vga", x86_family::vga_task, 0x202, (uint32*)cr3);
		task_add("prompt", kernel_shell_task, 0x202, (uint32*)cr3);

		// Begin multi-tasking
		task_enable_scheduler();
		task_preempt();
	}

	static void kernel_shell_task()
	{
		startup_welcome();

		while (true)
			kshell(0, nullptr);
	}
}
