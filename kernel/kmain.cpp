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
#include <tupai/syscall.hpp>
#include <tupai/task.hpp>
#include <tupai/kdebug.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/gdt.hpp>
	#include <tupai/i686/idt.hpp>
	#include <tupai/i686/interrupt.hpp>
	#include <tupai/i686/kbd.hpp>
	#include <tupai/i686/pit.hpp>
#endif

namespace tupai
{
	static void kernel_task_test_a();
	static void kernel_task_test_b();
	static void kernel_task_test_c();

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

	void kernel_welcome()
	{
		tty_write_str("\nWelcome to ");
		tty_write_str(SYSTEM_NAME_DECORATIVE);
		tty_write_str(" ");
		tty_write_str(SYSTEM_VERSION_MAJOR);
		tty_write_str(".");
		tty_write_str(SYSTEM_VERSION_MINOR);
		tty_write_str(".");
		tty_write_str(SYSTEM_VERSION_RELEASE);
		tty_write_str("\n");

		#if defined(CFG_SHOW_LICENSE_AT_BOOT)
			const char GPL3_LICENCE_SHORT[] = "Copyright (C) 2016 Joshua Barretto\n" \
			"This program comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are " \
			"welcome to\nredistribute it under certain conditions.\n\nYou should have received a copy" \
			" of the GNU\nGeneral Public License along with this program.\nIf not, see <http://www.gn" \
			"u.org/licenses/>.\n";

			tty_write('\n');
			tty_write_str(GPL3_LICENCE_SHORT);
		#endif

		tty_write('\n');
	}

	// Kernel early
	void kearly()
	{
		tty_init();
		kmain_write_check("Started kernel TTY");

		mempool_init((ubyte*)mempool_begin, mempool_size, 64); // At 16 MB, 1 MB in size, composed of blocks of 64 B
		kmain_write_check("Initiated dynamic memory pool");

		#if defined(SYSTEM_ARCH_i686)
			gdt_init();
			kmain_write_check("Initiated GDT");

			idt_init();
			kmain_write_check("Initiated IDT");

			interrupt_enable();
			kmain_write_check("Enabled interrupts");

			pit_init();
			kmain_write_check("Initiated PIT");

			kbd_init();
			kmain_write_check("Initiated keyboard");
		#endif

		syscall_init();
		kmain_write_check("Initiated syscall routine");

		task_init();
		kmain_write_check("Initiated task scheduler");

		kmain_write_check("Finished early kernel boot");
	}

	// Kernel main
	void kmain()
	{
		kmain_write_check("Main kernel boot complete");

		libk::printf("\nAdding test tasks A and B...\n");

		kassert(true == false);

		// Get EFLAGS and CR3
		uint32 cr3 = 0;
		uint32 eflags = 0x202;
	    //asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(cr3)::"%eax");
	    //asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(eflags)::"%eax");

		task_add("testa", kernel_task_test_a, eflags, (uint32*)cr3);
		task_add("testb", kernel_task_test_b, eflags, (uint32*)cr3);
		//task_add("testc", kernel_task_test_c, eflags, (uint32*)cr3);

		// Enable the scheduler
		//task_enable_scheduler();

		kernel_welcome();

		while (prompt() == 0);
	}

	static void kernel_task_test_a()
	{
		while (true)
		{
			//volatile uint32 a = 0;
			//while (a < 5000) a ++;

			libk::printf("I am A!\n");
		}
	}

	static void kernel_task_test_b()
	{
		while (true)
		{
			//volatile uint32 a = 0;
			//while (a < 5000) a ++;

			libk::printf("B is me!\n");
		}
	}

	static void kernel_task_test_c()
	{
		while (true)
		{
			//volatile uint32 a = 0;
			//while (a < 5000) a ++;

			libk::printf("I'm C!\n");
		}
	}
}
